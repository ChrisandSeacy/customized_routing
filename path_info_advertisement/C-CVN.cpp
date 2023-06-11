/*
 *　用于仿真TRPM算法：
 *
 *
 * */

#include "_mysql.h"
#include "_public.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>
#include <list>
#include <tr1/unordered_map>
#include <string>
#include <deque>
#include <set>

using namespace std;


//变量
CLogFile logfile; //日志变量
int shmid;        //共享内存标识符：存储当前所识别的csv文件 
vector<struct path_item> path_vec;   //用于接受csv信息的结构体数组
float threshold=1.0; //安全值的阈值:范围0-1，越小表示安全范围越高
char strddatetime[21]; //时间


//结构体
//节点的结构体
struct Node
{
   int node_id;
   char node_name[31];
   int trust_value;
};


//边的结构体
struct Arc
{
  int src_id;
  int dest_id;

  char src_name[31];
  char dest_name[31];

  int delay;
  int packet_loss;
  int throughput;
  int trust_value;
};




//仿照真正的路由表的item结构
struct routingTable_item
{
  char src[31];         //源地址
  char destination[31]; //目标地址
  char gateway[31];     //下一跳地址
  char genmask[31];     //子网掩码
  int flag;             
  int metric;
  int ref;
  int use;
  int iface;            //接口
};




struct current_info
{
  char current_fileName[301];   //最新生成的文件名字
  int pid;                      //当前进程的进程号码
};


//用于接csv文件
struct path_item
{
   int arc_id;
   char src[31];
   char dest[31];
   int delay;
   int packet_loss;
   int throughput;
   int trust_value;
};


//把filename的文件接到结构体数组中
bool LoadPath(const char* filename,vector<struct path_item>& path_vec);
float max(float a,float b)
{
   if(a>b) return a;
   else return b;

}

class CCVN
{
private:
   vector<struct Node> node_set; 
   vector<list<struct Arc> > arc_set;
   vector<struct routingTable_item> routingTable;  //routingTable


public:
  CCVN();
  CCVN(vector<struct Node>& nodeSet,vector<list<struct Arc> >& arcSet)
  {
     this->node_set=nodeSet;
     this->arc_set=arcSet;
  }

  //路由算法: 构建所有src到dest的路由表
  void caculateRouting(float weightDelay, float weightThroughput, float weightPacket_loss,float weightTrust)
  {
     for(int i=0;i<node_set.size();i++)
     {
        for(int j=0;j<node_set.size();j++)
        {
           if(i!=j)
           {
             caculate_item(node_set[i],node_set[j],weightDelay,weightThroughput,weightPacket_loss,weightTrust);        	
           }
        }
     } 
  }

 
//指定src和dest去填充路由表,这个函数是计算所有节点到达dest的routing_item条目，并添加到routingTable中
//返回值，返回的是从src有几条路径到达dest
//虽然填写的是src到dest，但是计算的是所有到达dest的routing_item

  void caculate_item(Node src,Node dest,float weightDelay, float weightThroughput, float weightPacket_loss,float weightTrust)
  { 
      //1. 构建所有节点的二元组
      vector<pair<float,float> > node_pair(node_set.size(),{0,0});
      
      node_pair[src.node_id]={0,weightTrust*(10-src.trust_value)};

      deque<int> deque_node;
      deque_node.push_back(src.node_id);

      while(!deque_node.empty())
      {
        int node_temp=deque_node.front();
      
        for(list<struct Arc>::iterator iter=arc_set[node_temp].begin();
                                       iter!=arc_set[node_temp].end();
                                       iter++){
           if(node_pair[iter->dest_id].first==0 && node_pair[iter->dest_id].second==0){
              float path_effective=weightDelay * iter->delay - weightThroughput *iter->throughput + weightPacket_loss * iter->packet_loss;
           
              float trust_temp=weightTrust * (10-node_set[iter->dest_id].trust_value);

              node_pair[iter->dest_id]={node_pair[iter->src_id].first+path_effective,\
                                        max(node_pair[iter->src_id].second,trust_temp)};
           }
           else //证明已经走过
           {
             float path_effective=weightDelay * iter->delay - weightThroughput *iter->throughput + weightPacket_loss * iter->packet_loss;

             float trust_temp=weightTrust * (10-node_set[iter->dest_id].trust_value);


             node_pair[iter->dest_id].first=min(node_pair[iter->dest_id].first,node_pair[iter->src_id].first+path_effective);
             node_pair[iter->dest_id].second=max(node_pair[iter->src_id].second,trust_temp);
           }

             deque_node.push_back(iter->dest_id);
	}
             deque_node.pop_front();
      }

      
      //2. 删除不满足条件的节点
       set<int> delete_node;  //便利node_pair 删除不符合条件的节点
       pair<float, float> target=node_pair[dest.node_id];

       for(int i=0;i<node_pair.size();i++)
       {
          if((node_pair[i].first>target.first) || (node_pair[i].second>(target.second * threshold)) && i!=dest.node_id)
          {
            delete_node.insert(i);
          }
       }   

      //3. 进行深度优先遍历
      vector<int> visit(node_set.size());
      dfs(src,src,dest,delete_node,visit);
  }





  //深度优先遍历算法
  void dfs(struct Node src, struct Node cur, struct Node dest, set<int>& delete_node, vector<int>& visit)
  {
      
      for(list<struct Arc>::iterator iter=arc_set[cur.node_id].begin();iter!=arc_set[cur.node_id].end();iter++)
      {
          if(iter->dest_id==dest.node_id) //find
          {
              visit[iter->dest_id]=iter->src_id;
              int temp=iter->dest_id;
              while(visit[temp]!=src.node_id)
              {
                   temp=visit[temp];
              }
              
              struct routingTable_item item;
              memset(&item,0,sizeof(struct routingTable_item));
              strcpy(item.src,src.node_name);
              strcpy(item.destination,dest.node_name);
              strcpy(item.gateway,node_set[temp].node_name);
              strcpy(item.genmask,"255.255.255.0");
 
              routingTable.push_back(item);
              return;
          }
  
          if(delete_node.find(iter->dest_id)==delete_node.end())
          {
             visit[iter->dest_id]=cur.node_id;
             dfs(src,node_set[iter->dest_id],dest,delete_node,visit);
          }

     }
        return;
  } 



  //打印路由表
  void printRouting()
  {
    for(int i=0;i<routingTable.size();i++)
    {
      cout<<routingTable[i].src<<"\t"<<routingTable[i].destination<<"\t"<<routingTable[i].gateway<<"\t"<<routingTable[i].genmask<<endl;

    }
  }
 
  //生成路由表文件：
  bool RoutingTableFile(char* outpath, char* datafmt)
  { 
	memset(strddatetime,0,sizeof(strddatetime));
   	LocalTime(strddatetime,"yyyymmddhh24miss");
	CFile File;
        char strFileName[301];
  	char fileMemory[301];

        memset(strFileName,0,301);
        memset(fileMemory,0,301);

        sprintf(fileMemory,"RouteTable_%s_%d.%s",strddatetime,getpid(),datafmt);

        sprintf(strFileName,"%s/%s",outpath,fileMemory);

       //打开文件
        if(File.OpenForRename(strFileName,"w")==false)
        {
        	printf("File.OPenForRename(%s) failed.\n",strFileName);
        	return false;
  	}
	
        for(int i=0;i<routingTable.size();i++)
   	{     
        	if(strcmp(datafmt,"csv")==0)     
                {        
                 File.Fprintf("%s,%s,%s,%s,%d,%d,%d\n",routingTable[i].src,routingTable[i].destination,routingTable[i].gateway,routingTable[i].genmask,routingTable[i].flag,routingTable[i].metric,routingTable[i].ref);
     		}
   	}
    File.CloseAndRename(); 
   


        if((shmid=shmget(0x5001,sizeof(struct current_info),0640|IPC_CREAT))==-1)
        {
        	printf("生成共享内存0x5001失败\n");
                return -1;
        }

        struct current_info* cur_file=0;

   	if((cur_file=(struct current_info*)shmat(shmid,0,0))==(void*)-1)
   	{
     		logfile.Write("绑定共享内存0x5001失败\n");
     		return -1;
   	}

   	strcpy(cur_file->current_fileName,fileMemory);
   	cur_file->pid=getpid();
  //     cout<<"share memory is"<<fileMemory;
        shmdt(cur_file);
   }
};







int main(int argc, char* argv[]){
 
  if(argc!=2)
  {
  	printf("parameter number error:\n");
     	printf("1. user_id");
  }
 
  //打开文件，构建表
  //1.打开mysql，构建nodeset
  vector<struct Node> nodeset;
  tr1::unordered_map<string,int> nameToid;
  
  struct Node node;
  connection conn;
  if(conn.connecttodb("127.0.0.1,root,Xiexiaoxuan79064-,mysql,3306","utf8")!=0){
    printf("connect database failed.\n%s\n", conn.m_cda.message);
    return -1;
  }

  sqlstatement stmt(&conn); //操作sql的对象

  stmt.prepare("select id, name, trust_value from routingnode");
  

  stmt.bindout(1,&node.node_id);
  stmt.bindout(2,node.node_name,50);
  stmt.bindout(3,&node.trust_value);

  //执行路由算法，生成路由表
  if(stmt.execute()!=0)
  {
    printf("stmt.execute() failed\n");
    return -1;
  }

  while(true) 
  {
    string temp_name;
    memset(&node,0,sizeof(Node));
    if(stmt.next()!=0) break; //提取错误
    nodeset.push_back(node);    
 
    temp_name=node.node_name;
    nameToid[temp_name]=node.node_id;
  }
  
  //2.1 从共享内存中找出最新的文件名字
   if((shmid=shmget(0x5000,sizeof(struct current_info),0640|IPC_CREAT))==-1)
    {
      logfile.Write("生成共享内存0x5000失败\n");
      return -1;
    }


   struct current_info* cur_file=0;

   if((cur_file=(struct current_info*)shmat(shmid,0,0))==(void*)-1)
   {
     logfile.Write("绑定共享内存0x5000失败\n");
     return -1;
   }
  
   //2.2 把文件加载到这个vec中
   LoadPath(cur_file->current_fileName,path_vec);   
   
   //2.3 构建拓扑
   vector<list<struct Arc> > arcset(nodeset.size());     
 
   for(int i=0;i<path_vec.size();i++)
   {
      struct Arc temp_arc;
      memset(&temp_arc,0,sizeof(struct Arc));
      
      string found_src=path_vec[i].src;
      string found_dest=path_vec[i].dest;



      temp_arc.src_id=nameToid[found_src];
      temp_arc.dest_id=nameToid[found_dest];
      
      strcpy(temp_arc.src_name,path_vec[i].src);
      strcpy(temp_arc.dest_name,path_vec[i].dest); 
      
      temp_arc.delay=path_vec[i].delay;
      temp_arc.packet_loss=path_vec[i].packet_loss;
      temp_arc.throughput=path_vec[i].throughput;    
      temp_arc.trust_value=path_vec[i].trust_value;
    
      arcset[temp_arc.src_id].push_back(temp_arc);
   }   
  

  //把路由表生成文件
  CCVN route(nodeset,arcset);
  
  char sentence[80];
  sprintf(sentence,"select weightDelay, weightThroughtput, weightPacket_loss,weightTrust from user_customized where userid=%s",argv[1]);

  stmt.prepare(sentence);

  float weightDelay=0.0;
  float weightThroughtput=0.0;
  float weightPacket_loss=0.0;
  float weightTrust=0.0;

  
  stmt.bindout(1,&weightDelay);
  stmt.bindout(2,&weightThroughtput);
  stmt.bindout(3,&weightPacket_loss);
  stmt.bindout(4,&weightTrust);
  
 
  if(stmt.execute()!=0)
  {
    printf("stmt.execute() failed\n");
    return -1;
  }
  
  if(stmt.next()!=0)
  {
    printf("get node_num from routingnode failed!\n");
    return -1;
  }


  route.caculateRouting(weightDelay,weightThroughtput,weightPacket_loss,weightTrust);
 // route.printRouting();
  route.RoutingTableFile("/routing/routeTable","csv");

}



//把文件中的内容加载到vector<struct path_item> path_vec中
bool LoadPath(const char* filename,vector<struct path_item>& path_vec)
{
     CFile File;
     CCmdStr cmdStr;  //定义拆分字符的对象     
     struct path_item pathItem; 
     
     //打开指定的文件
     if(File.Open(filename,"r")==false)
     {
        printf("open file(%s) failed\n",filename);
        return false;
     }   
 
     char strBuffer[301];
     int row_count=0;

     while(true)
     {
      //获取文件的第一行
      if(File.Fgets(strBuffer,300,true)==false) break; 
     
      //第一行要去除
      if(row_count==0) {
         row_count++;
         continue;
      }
 
     //把获得的一行进行拆分
     cmdStr.SplitToCmd(strBuffer,",");
     memset(&pathItem,0,sizeof(struct path_item));


     cmdStr.GetValue(0,&pathItem.arc_id);
     cmdStr.GetValue(1,pathItem.src,30);
     cmdStr.GetValue(2,pathItem.dest,30);
     cmdStr.GetValue(3,&pathItem.delay);
     cmdStr.GetValue(4,&pathItem.packet_loss);
     cmdStr.GetValue(5,&pathItem.throughput);
     cmdStr.GetValue(6,&pathItem.trust_value);
     
     path_vec.push_back(pathItem);
     
     }

    return true;
}
