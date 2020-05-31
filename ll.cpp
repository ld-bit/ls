#include<iostream>
#include<stack>
#include<dirent.h>
#include<unistd.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>
#include<string.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
using namespace std;
class Info{
friend class  LL;
public:
Info(char* path)
{ 
  lstat(path,&buf);
  memcpy(_path,path,sizeof(_path));
}
~Info()
{
}
private:
void OutPutInfo()
{
 file_type();
 printf(" %ld ",buf.st_nlink);
 struct passwd *ps=getpwuid(buf.st_uid);
 printf("%s ",ps->pw_name);
 struct group* gr=getgrgid(buf.st_gid);
 printf("%s ",gr->gr_name);
 printf("%5ld ",buf.st_size);
 struct tm* tim=localtime(&buf.st_mtime);
 char* date[13];
 date[0]="";
 date[1]="January";
 date[2]="February";
 date[3]="March";
 date[4]="April";
 date[5]="May";
 date[6]="June";
 date[7]="July";
 date[8]="August";
 date[9]="September";
 date[10]="October";
 date[11]="November";
 date[12]="December";
 printf("%3s ",date[tim->tm_mon+1]) ;
 printf("%2d ",tim->tm_mday);
 if(tim->tm_hour<10)
 {
   printf("0");
   printf("%d",tim->tm_hour);
 }
 else 
 printf("%2d",tim->tm_hour);
 printf(":");
 if(tim->tm_min<10)
 {
   printf("0");
   printf("%d",tim->tm_min);
 }
 else 
 printf("%2d",tim->tm_min);
 printf(" ");
 char buf[256]={'\0'};
 int len=0;
 if((len=readlink(_path,buf,256))!=-1)
 {
   printf("%s -> ",_path);
    printf("%s",buf);
 }
 else 
 printf("%s",_path);
 printf("\n");
}
void file_type()
{
mode_t mode=buf.st_mode;
char buf[11]="?---------";
switch(mode&S_IFMT){
case S_IFIFO : buf[0]='p'; break;
case S_IFDIR : buf[0]='d'; break ;
case S_IFCHR : buf[0]='c'; break ;
case S_IFBLK : buf[0]='b'; break ;
case S_IFREG:  buf[0]='-';  break ;
case S_IFLNK : buf[0]='l'; break ;
case S_IFSOCK: buf[0]='s'; break ;
}
if(mode&S_IRUSR) buf[1]='r';
if(mode&S_IWUSR) buf[2]='w';
if(mode&S_IXUSR) buf[3]='x';


if(mode&S_IRGRP) buf[4]='r';
if(mode&S_IWGRP) buf[5]='w';
if(mode&S_IXGRP) buf[6]='x';

if(mode&S_IROTH) buf[7]='r';
if(mode&S_IWOTH) buf[8]='w';
if(mode&S_IXOTH) buf[9]='x';
printf("%s",buf);
}
struct stat buf;
char _path[256];
};
class  LL{
public:
void PrintDirInfo()
{
 getDirInfo();
 while(!sk.empty())  
 {
  Info info(sk.top());
  info.OutPutInfo();
  sk.pop();
 }
}
private:
void getDirInfo()
{
   DIR* pdir= opendir(".");
   if(pdir==NULL)
   {
     perror("opendir"),exit(-1);
   }
   struct dirent* dir;
   stack<char*>help;
   char buf[256]={'\0'};
   int len=0;
   while((dir=readdir(pdir))!=NULL)
   {
      if(dir->d_name[0]=='.') 
        continue;
      if(sk.empty())
      {
       sk.push(dir->d_name);
      }
      else 
      {
        while(!sk.empty()&&strcmp(dir->d_name,sk.top())>0)
        {
          help.push(sk.top());
          sk.pop();
        }
        sk.push(dir->d_name);
        while(!help.empty())
        {
          sk.push(help.top());
          help.pop();
        }
      }
   }
   closedir(pdir);
}
stack<char*>sk;
};
int main()
{
 LL l; 
 l.PrintDirInfo();
}
