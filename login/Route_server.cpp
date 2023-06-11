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
#include "_mysql.h"
#include "_public.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


using namespace std;

int shmid;

struct arc{
 char src[100];
 char dest[100];
};

struct current_info
{
  char current_fileName[301];   //最新生成的文件名字
  int pid;                      //当前进程的进程号码
};


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

         if(fork()==0)
         {
         	execl("/routing/path_info_advertisement/C-CVN","C-CVN",buffer,(char*)0);
         }
         else
         {
        
         	int stat=0;
         	wait(&stat);

         //要把文件名字传递回去  
         if((shmid=shmget(0x5001,sizeof(struct current_info),0640|IPC_CREAT))==-1)
    	 {
      		printf("生成共享内存0x5001失败\n");
      		return -1;
         }

         //最新的file的名字
         struct current_info* cur_file=0;
   
         if((cur_file=(struct current_info*)shmat(shmid,0,0))==(void*)-1)
         {
         	printf("绑定共享内存0x5001失败\n");
                return -1;
         }
   
          strcpy(buffer,cur_file->current_fileName);
          if ( (iret=send(evs[ii].data.fd,buffer,strlen(buffer),0))<=0) // 向客户端发送响应结果。
           { perror("send"); break; }
            printf("发送：%s\n",buffer);
           cout<<buffer<<endl;       
 
         // 第6步：关闭socket，释放资源。
         close(evs[ii].data.fd);
         }
    }    
  }
 }
}
