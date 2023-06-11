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
int threshold=3; //安全值的阈值

//结构体
//节点的结构体
struct Node
{
   int node_id;
   char node_name[31];
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


class TRPM
{
private:
   vector<struct Node> node_set; 
   vector<list<struct Arc> > arc_set;
   vector<struct routingTable_item> routingTable;  //routingTable


public:
  TRPM();
  TRPM(vector<struct Node>& nodeSet,vector<list<struct Arc> >& arcSet)
  {
     this->node_set=nodeSet;
     this->arc_set=arcSet;
  }

  //路由算法: 构建所有src到dest的路由表
  void caculateRouting()
  {
    struct Node dest;
    memset(&dest,0,sizeof(struct Node));
    dest.node_id=10;
    strcpy(dest.node_name,"sd");
    
    for(int i=0;i<node_set.size();i++)
    {
      caculate_item(node_set[i],dest);
    } 
    return; 
   
  }

 
//指定src和dest去填充路由表,这个函数是计算所有节点到达dest的routing_item条目，并添加到routingTable中
//返回值，返回的是从src有几条路径到达dest
//虽然填写的是src到dest，但是计算的是所有到达dest的routing_item

  int caculate_item(Node src,Node dest)
  { 
    
    vector<list<struct Arc> > arc_set_temp=arc_set;

    int srcId=src.node_id;
    int destId=dest.node_id;
    int count=0;

    deque<int> node_deque;
    set<int> node_id_set;


    node_deque.push_back(srcId);
    node_id_set.insert(srcId);    
    vector<int> last_hop(node_set.size(),-1); 
    vector<int> dest_last;

   //正向寻找
    while(!node_deque.empty())
    {
       int cur_node=node_deque.front();
      

       for(list<struct Arc>::iterator iter=arc_set[cur_node].begin();iter!=arc_set[cur_node].end();iter++)
       {
          
          if(iter->trust_value<threshold)
          {
             continue;
          }
          
          else if(iter->dest_id==destId) //发现一条路径
          {
            dest_last.push_back(cur_node);
            continue;
          }
   
          else if(node_id_set.find(iter->dest_id)!=node_id_set.end()) //每个信息只能接受一次
          {
             continue;
          }

          else
          {
             node_deque.push_back(iter->dest_id);
             node_id_set.insert(iter->dest_id);
             last_hop[iter->dest_id]=cur_node;
          }
       }   
      node_deque.pop_front();            
    }
     
   //反向扩散
   for(int i=0;i<dest_last.size();i++)
   {
     int cur_node=dest_last[i];

     while(last_hop[cur_node]!=srcId && last_hop[cur_node]!=-1)
     {
       cur_node=last_hop[cur_node];
     }
     
     struct routingTable_item item;
     memset(&item,0,sizeof(routingTable_item));

     strcpy(item.src,node_set[srcId].node_name);
     strcpy(item.destination,node_set[destId].node_name);
      
     if(last_hop[cur_node]==-1)  strcpy(item.gateway,node_set[destId].node_name);
     else  strcpy(item.gateway,node_set[cur_node].node_name);

     strcpy(item.genmask,"255.255.255.255");
        
        int flag=0;
        int metric=0;
        int ref=0;
        int use=0;
        int iface=0;  
     this->routingTable.push_back(item);
   }
   return count;  
}


  //打印路由表
  void printRouting()
  {
    for(int i=0;i<routingTable.size();i++)
    {
      cout<<routingTable[i].src<<"|"<<routingTable[i].destination<<"|"<<routingTable[i].gateway<<"|"<<routingTable[i].genmask<<endl;

    }
  }
 
  //生成路由表文件
  void RoutingTableFile(char* outpath, char* datafmt);


};







int main(int argc, char* argv[]){
  
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

  stmt.prepare("select id, name from routingnode");
  

  stmt.bindout(1,&node.node_id);
  stmt.bindout(2,node.node_name,50);

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

  TRPM route=TRPM(nodeset,arcset);

  route.caculateRouting();
  route.printRouting();


  return 0;
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
