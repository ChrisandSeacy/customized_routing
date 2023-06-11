/*
 * 程序名：server.cpp，此程序用于路由注册服务器
 * 作者：chris Alt
 * 
 * 执行：./server + 端口号
 *       ./client + 服务器ip + 服务器端口号 +字符串（添加的节点ip，节点注册密码，源节点1，目的节点2，源节点2，目的节点2，……）
 *
 * 解释： 新增的节点和边都会注册到mysql数据库中，节点数据库是routingnode，边数据库是routingarc
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "_mysql.h"
#include <sys/epoll.h>

using namespace std;

struct arc{
 char src[100];
 char dest[100];
};

bool ischeck(string usrname, string passwd)
{
 return true;
}


void split_info(char* buffer,int size, vector<string>& res)
{
  string temp="";
  
  for(int i=0;i<size;i++)
  {
    if(*(buffer+i)!=',')
    {
      temp+=(*(buffer+i));
    }     
    else
    {
      res.push_back(temp);
      temp="";
    }
  }
   if(temp!="") res.push_back(temp);  
   return;
}



int main(int argc,char *argv[])
{
  if (argc!=2)
  {
    printf("Using:./demo02 port\nExample:./demo02 5005\n\n"); return -1;
  }

  // 第1步：创建服务端的socket。
  int listenfd;
  if ( (listenfd = socket(AF_INET,SOCK_STREAM,0))==-1) { perror("socket"); return -1; }
  
  // 第2步：把服务端用于通讯的地址和端口绑定到socket上。
  struct sockaddr_in servaddr;    // 服务端地址信息的数据结构。
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
  servaddr.sin_port = htons(atoi(argv[1]));  // 指定通讯端口。
  if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 )
  { perror("bind"); close(listenfd); return -1; }
 
  // 第3步：把socket设置为监听模式。
  if (listen(listenfd,5) != 0 ) { perror("listen"); close(listenfd); return -1; }
  


  // 第4步：创建epoll句柄
  int epollfd=epoll_create(1);
  
  //为监听的socket设置可读事件
  struct epoll_event ev;
  ev.events=EPOLLIN;
  ev.data.fd=listenfd;
  

  //准备好ev之后，将它加入epoll
  epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);




 
  int iret;
  char buffer[102400];
   
  connection conn;
  if(conn.connecttodb("127.0.0.1,root,Xiexiaoxuan79064-,mysql,3306","utf8")!=0){
     printf("connect database failed.\n%s\n", conn.m_cda.message);
     return -1;
  }

  sqlstatement stmt(&conn); //操作sql的对象


  
  struct epoll_event evs[10];  //放返回的事件




  // 第5步：与客户端通讯，接收客户端发过来的报文后，回复ok。
  while (1)
  {
    int infds=epoll_wait(epollfd, evs, 10 , -1);    

    if(infds<0)
    {
       printf("select() failed\n");
       break;
    }

    if(infds==0)
    {
       printf("select() timeout\n");
       continue;
    }
 
    //遍历epoll返回的时间数组
    for(int ii=0;ii<infds;ii++)
    {
       printf("events=%d, data.fd=%d\n",evs[ii].events,evs[ii].data.fd);

       
       //如果是listen事件
       if (evs[ii].data.fd==listenfd)
       {
          struct sockaddr_in client;
          socklen_t len=sizeof(client);
          int clientsock = accept(listenfd,(struct sockaddr*)&client,&len);
          if(clientsock<0) 
          {
              printf("accept client(socket=%d ok)\n",clientsock);
          }

          //为新客户准备可读时间，添加到epoll中
          ev.data.fd=clientsock;
          ev.events=EPOLLIN;
          epoll_ctl(epollfd,EPOLL_CTL_ADD,clientsock,&ev);

       }

        else{
 
         //recv  
         memset(buffer,0,sizeof(buffer));
         if ((iret=recv(evs[ii].data.fd,buffer,sizeof(buffer),0))<=0) // 接收客户>端的请求报文。
         {
           printf("iret=%d\n",iret); break; 
         }
         printf("接收：%s\n",buffer);



         //buffer
         //将接受的信息存到vec_info中
         vector<string> vec_info;
         split_info(buffer,strlen(buffer),vec_info);


         // ischeck 验证密码的合理性
         if(ischeck(vec_info[0],vec_info[1])==false)
         {
           printf("node add failed\n");
           return -1;
         }  
    
         //确定routingnode有几行，确定idnode
         int idnode;
         stmt.prepare("select count(*) from routingnode");

         stmt.bindout(1,&idnode);
    
         if(stmt.execute()!=0)
         { 
            printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
            return -1;
         }

        if(stmt.next()!=0)
        {
            printf("get idnode from routingid failed!\n");
            return -1;
        } 

        //插入idnode
        stmt.prepare("\
                  insert into routingnode(id,name) values(:1,:2)");
  
         char node[100];

         stmt.bindin(1,&idnode);
         stmt.bindin(2,node,99);

         strcpy(node,vec_info[0].c_str());




         if(stmt.execute()!=0)
         {
             printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
             return -1;
         }

         printf("成功插入了%ld条记录\n",stmt.m_cda.rpc);
         conn.commit();   


        //开始插入边
         for(int i=2;i<vec_info.size();i=i+2)
         {
            struct arc temp_arc;
            memset(&temp_arc,0,sizeof(struct arc));
      
            //寻找表有几行，确定边的数量
            int id;
            stmt.prepare("select count(*) from routingarc");

            stmt.bindout(1,&id);

            if(stmt.execute()!=0)
            {
               printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
               return -1;
            }

            if(stmt.next()!=0)
            {
               printf("get idnode from routingid failed!\n");
               return -1;
             }     
     
            stmt.prepare("\
                  insert into routingarc(id,src,dest) values(:1,:2,:3)");

            stmt.bindin(1,&id);
            stmt.bindin(2,temp_arc.src,99);
            stmt.bindin(3,temp_arc.dest,99);

            strcpy(temp_arc.src,vec_info[i].c_str());
            strcpy(temp_arc.dest,vec_info[i+1].c_str());

            printf("%s %s\n",temp_arc.src,temp_arc.dest);
  
            if(stmt.execute()!=0)
            {
                printf("stmt.execute() failed.\n%s\n%d\n%s\n",stmt.m_sql,stmt.m_cda.rc,stmt.m_cda.message);
                return -1;
            }

            printf("成功插入了%ld条记录\n",stmt.m_cda.rpc);
            conn.commit();
        }


        strcpy(buffer,"ok");
         if ( (iret=send(evs[ii].data.fd,buffer,strlen(buffer),0))<=0) // 向客户端发送响应结果。
           { perror("send"); break; }
            printf("发送：%s\n",buffer);
         
 
         // 第6步：关闭socket，释放资源。
         close(evs[ii].data.fd);
    }    
  }
 }
}
