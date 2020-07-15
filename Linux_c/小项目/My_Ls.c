#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pwd.h>
#include<string.h>
#include<grp.h>
#include<fcntl.h>
#include<sys/types.h>
#include<dirent.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<signal.h>
#define DIR_COL    "\033[1;34m"
#define LINK_COL   "\033[1;30m"
#define S_COL "\033[1;37m"
#define F_COL    "\033[1;36m"
#define O_COL "\033[1;35m"
#define NONE    "\033[m"
#define MAXLEN 100
#define PARAMNO 0
#define PARAMA 1
#define PARAML 2
#define PARAMR 4
char *filename[50000];                          //记录目录下文件和子目录的名称
char pathname[50][50];                          //目录与文件名称
int count;                                      //记录目录下的文件和子目录的个数
int maxlen;                                     //记录目录下最长的文件名
int param_flag = PARAMNO;                       //记录权限位
int leave_len = MAXLEN;                         //记录每行剩余的长度
void  getdir_name(char dir_name[]);             //获得目录下文件和子目录名称的函数
void quick_sort(char *filename1[],int l,int r); //名称排序
void display_a(char *filename,char *pathname);  //-a
void print_filedata(char *filename);            //-l
void Signal_Bye_Ctrlc();                        //屏蔽ctrl + c
void display_dir(int flag,char*name);           //-R

int main(int argc,char *argv[])
{
    Signal_Bye_Ctrlc();
    //解析命令行参数
    int j = 0;
    int param_count = 0;
    char param[20];                   //保存命令行参数
    memset(pathname,'\0',sizeof(pathname));
    memset(param,'\0',sizeof(param));
    int pathname_count = 0;
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            for(int k = 1;k < (int)strlen(argv[i]); k++)
            {
                param[j] = argv[i][k];
                j++;
            }
            param_count++;
        }
    }
    //解析ls -后面的参数
    for(int i = 0; i <= param_count ; i++)
    {
        if(param[i] == 'a')
        {
            param_flag |= PARAMA;
        }
        else if(param[i] == 'l')
        {
            param_flag |= PARAML;
        }
        else if(param[i] == 'R')
        {
            param_flag |= PARAMR;
        }
        else
        {
            param_flag |= PARAMNO;
        }
    }
    //解析文件或目录名
    int i = 1;
    if(i < argc)
    {
        while(argv[i][0] == '-')
        {
            i++;
            //防止只输入./a.out -a / -l/ -al时出现数组访问越界的情况
            if(argc<=i)
                break;
        }
    }
    int k = 0;
    for(; i < argc;i++)
    {
        pathname_count ++;
        strcpy(pathname[k++],argv[i]);
    }
//  对应单独输入ls的情况
    if(pathname_count == 0)
    {
        pathname_count ++;
        strcpy(pathname[0],".");
    }
    //判断文件或目录名是否存在
    for(int i = 0; i < pathname_count ; i++)
    {
        struct stat buf;
        if(lstat(pathname[i],&buf) != -1)//判断是否为单个文件
        {
            if(S_ISDIR(buf.st_mode))
            {
                //多个目录时需要打印出目录名称
                if(pathname_count != 1)
                {
                    printf("%s:\n",pathname[i]);
                }
                switch(param_flag)
                {
                    case 1:{//-a
                               if(strlen(pathname[i]) == 0)
                               {
                                    strcpy(pathname[i],".");
                               }
                                getdir_name(pathname[i]);
                                quick_sort(filename,0,count-1);
                                for(int j = 0; j < count ;j++)
                                    display_a(filename[j],pathname[i]);
                                printf("\n");
                                break;
                           }
                    case 2:{//-l
                               if(strlen(pathname[i]) == 0)
                               {
                                    strcpy(pathname[i],".");
                               }
                               getdir_name(pathname[i]);
                               quick_sort(filename,0,count-1);
                               for(int j = 0; j < count ;j++)
                               {
                                   if(strcmp(filename[j],".") != 0 && strcmp(filename[j],"..") != 0)
                                        print_filedata(filename[j]);
                               }
                               break;

                           }
                    case 3:{//-al
                               if(strlen(pathname[i]) == 0)
                               {
                                    strcpy(pathname[i],".");
                               }
                               getdir_name(pathname[i]);
                               quick_sort(filename,0,count-1);
                               for(int j = 0; j < count ;j++)
                               {
                                    print_filedata(filename[j]);
                               }
                               break;
                           }
                    case 0:{// 没有参数
                                getdir_name(pathname[i]);
                                quick_sort(filename,0,count-1);
                                for(int j = 0; j < count ;j++)
                                {
                                   if(strcmp(filename[j],".") != 0 && strcmp(filename[j],"..") != 0)
                                        display_a(filename[j],pathname[i]);
                                }
                                printf("\n");
                                break;
                           }
                    case 4:{//-R
                            //   getdir_allname(pathname[i]);
                              display_dir(4,"/");
                               break;
                            
                           }
                }
                
            }
            else
            {
                memset(pathname,'.',sizeof(pathname));
               print_filedata(pathname[i]);
            }
        }
        else
        {
            printf("pathname is not file/DIR,line :%d\n",__LINE__);
        }
    }
    return 0;
}

//获取目录下所有文件的名称
void  getdir_name(char dir_name[])
{
    DIR *dir;
    struct dirent * filedata;
    int i = 0;
    dir = opendir(dir_name);
    if(dir == NULL)
    {
        printf("dir open error!line : %d\n",__LINE__);
        return ;
    }
    /*if(readdir(dir) == NULL)
    {
        printf("dir read error!line : %d\n",__LINE__);
        return ;
    }
    */
    while( (filedata = readdir(dir)) != NULL)
    {

        filename[i] = (char *)malloc(sizeof(char)*200);
        if((int)strlen(filedata->d_name) > maxlen)
            maxlen = strlen(filedata->d_name);
        strcpy(filename[i],filedata->d_name);
        i++,count++;
    }
    closedir(dir);
}
//对文件名进行排序
void quick_sort(char *filename[],int l,int r)
{
    if( l >= r ) return;
    char* x = (char *)malloc(sizeof(char)*200);
    strcpy(x,filename[l+r>>1]);
    int i = l-1, j = r+1;
    while(i < j)
    {
        do i++;while(strcasecmp(x,filename[i]) > 0);
        do j--;while(strcasecmp(x,filename[j]) < 0);
        if( i < j )
        {
            char *temp;
            temp = (char*)malloc(sizeof(char)*200);
            strcpy(temp,filename[i]);
            strcpy(filename[i],filename[j]);
            strcpy(filename[j],temp);
            free(temp);
        }
    }
    quick_sort(filename,l,j);
    quick_sort(filename,j+1,r);
    free(x);
}

void display_a(char *filename,char *pathname)
{
    struct stat file;
    char temp[100];
    int len = strlen(filename);
    if(leave_len < maxlen)
    {
        printf("\n");
        leave_len = MAXLEN;
    }
    //文件是什么类型，按颜色输出
    memset(&file,'\0',sizeof(file));
    memset(temp,'\0',sizeof(temp));
    strcpy(temp,pathname);
    strcat(temp,"/");
    strcat(temp,filename);
    if(stat(temp,&file) == -1)
    {
        printf("stat error!line : %d\n",__LINE__);
    }
    //文件的属性
    if(S_ISLNK(file.st_mode)) 
        printf(LINK_COL"%-s"NONE,filename);
    else if(S_ISREG(file.st_mode)) 
        printf("%-s",filename);
    else if(S_ISDIR(file.st_mode)) 
        printf(DIR_COL"%-s"NONE,filename);
    else if(S_ISCHR(file.st_mode)) 
        printf(S_COL"%-s"NONE,filename);
    else if(S_ISBLK(file.st_mode)) 
        printf(S_COL"%-s"NONE,filename);
    else if(S_ISFIFO(file.st_mode)) 
        printf(F_COL"%-s"NONE,filename);
    else if(S_ISSOCK(file.st_mode)) 
        printf(O_COL"%-s"NONE,filename);
    printf("  ");   
    leave_len -=  len+2;
}

void print_filedata(char *filename)
{
    struct stat file ;
    //构造pathname /filename
    char statname[50];
    memset(statname,'\0',sizeof(statname));
    //strcpy(statname,"/");
    strcpy(statname,(const char*)pathname);
    strcat(statname,"/");
    strcat(statname,filename);
    if(stat(statname,&file) == -1)
    {
        printf("stat error!line : %d\n",__LINE__);
    }
    char str_mode[11];
    memset(str_mode,'-',sizeof(str_mode));
    //文件的属性
    if(S_ISLNK(file.st_mode)) str_mode[0] = 'l';
    if(S_ISREG(file.st_mode)) str_mode[0] = '-';
    if(S_ISDIR(file.st_mode)) str_mode[0] = 'd';
    if(S_ISCHR(file.st_mode)) str_mode[0] = 'c';
    if(S_ISBLK(file.st_mode)) str_mode[0] = 'b';
    if(S_ISFIFO(file.st_mode)) str_mode[0] = 'p';
    if(S_ISSOCK(file.st_mode)) str_mode[0] = 's';
    
    //文件的权限
    //用户
    if(file.st_mode & S_IRUSR) str_mode[1] = 'r';
    if(file.st_mode & S_IWUSR) str_mode[2] = 'w';
    if(file.st_mode & S_IXUSR) str_mode[3] = 'x';
    //用户组
    if(file.st_mode & S_IRGRP) str_mode[4] = 'r';
    if(file.st_mode & S_IWGRP) str_mode[5] = 'w';
    if(file.st_mode & S_IXGRP) str_mode[6] = 'x';        
    //其他人
    if(file.st_mode & S_IROTH) str_mode[7] = 'r';
    if(file.st_mode & S_IWOTH) str_mode[8] = 'w';
    if(file.st_mode & S_IXOTH) str_mode[9] = 'x';
    str_mode[10] = '\0';
    //时间处理
    struct tm *filetm = localtime(&file.st_mtim .tv_sec);
    char timebuf[20];
    memset(timebuf,'\0',sizeof(timebuf));
    sprintf(timebuf,"%2d月  %2d %02d:%02d",filetm->tm_mon+1 ,filetm->tm_mday,filetm->tm_hour,filetm->tm_min);
    printf("%s %4ld %s %3s  %5ld %12s %-10s\n",str_mode,file.st_nlink,getpwuid(file.st_uid)->pw_name,getgrgid(file.st_gid)->gr_name
           ,file.st_size,timebuf,filename);
}

void display_dir(int flag,char*name) 
{
    DIR * dir;
    struct dirent  *ptr;
    int i,count1 = 0;
    struct stat buf;
    char name_dir[10000];
    if(chdir(name)<0)                              //将输入的目录改为当前目录，下面操作的前提
    {

    }
    if(getcwd(name_dir,10000)<0)
    {

    }
    printf("%s:\n",name_dir);
    dir = opendir(name_dir);     //用新获得的路径打开目录
    if(dir==NULL)
    {
        printf("opendir error! line:%d",__LINE__);
    }
    while((ptr = readdir(dir))!=NULL)
    {
        count1++;
    }
    closedir(dir);
 
    //动态数组
    char**filenames =(char**)malloc(count1*sizeof(char*));    //要进行初始化 
    memset(filenames,0,sizeof(char*)*count1);
    for(i=0;i<count1;i++)
    {
        filenames[i]=(char*)malloc(256*sizeof(char));
        memset(filenames[i],0,sizeof(char)*256);
    }
 
    dir = opendir(name_dir);
    for(i=0;i<count1;i++)
    {
        ptr = readdir(dir);
        if(ptr == NULL)
        {
            printf("readdir error! line:%d",__LINE__);
        }
        if(strcmp(ptr->d_name,"..") != 0 && strcmp(ptr->d_name,".") != 0)
            printf("%s ",ptr->d_name);
        printf("\n");
        strcat(filenames[i],ptr->d_name);    //这里要注意用之前的初始化
    }
    if(flag>3)
    {                         //递归实现核心部分
      for(i=0;i<count1;i++)
      {
 
          if(lstat(filenames[i],&buf)==-1)
          {
              //防止没有权限的文件打不开
              return ;
              printf("lstat error! line:%d",__LINE__);
              
          }
          if(strcmp(filenames[i],"..")==0)
          continue;
          if(strcmp(filenames[i],".")==0)
          continue;
          if(S_ISDIR(buf.st_mode))
          {
            display_dir(flag,filenames[i]);
          }
          else if(!S_ISDIR(buf.st_mode))
          {
             continue;
          }
            chdir("../");          //处理完一个目录后返回上一层
        }
 
    }
    for(i=0;i<count1;i++)
    {
        free(filenames[i]);
    }
    free(filenames);
    closedir(dir);          //在函数开始时打开，结束时关闭
}

void Signal_Bye_Ctrlc()
{
    sigset_t Bye;
    sigemptyset(&Bye);//将信号集设置为空
    sigaddset(&Bye,SIGINT);//在set信号集中加入ctrl + c
    sigprocmask(SIG_BLOCK,&Bye,NULL);

}
