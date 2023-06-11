#include "_ftp.h"

Cftp ftp;

struct current_info
{
  char current_fileName[301];   //最新生成的文件名字
  int pid;                      //当前进程的进程号码
};


int main(int args, char* argv[])
{
  
  if(ftp.login("106.52.81.97:21","ftpuser","xiexiaoxuan79064")==false)
  {
      cout<<"login false"<<endl;
      return -1;
  }

  char routermemory[301];
  char sourceiddress[301];

  sprintf(sourceiddress,"/routing/routeTable/%s",argv[1]);
  sprintf(routermemory,"/routing/routerMemory/%s",argv[1]);
  cout<<routermemory<<endl;

  
  ftp.get(sourceiddress,routermemory);
  ftp.logout(); 
  return 0;

}

