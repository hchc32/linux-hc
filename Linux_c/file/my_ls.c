#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<dirent.h>
#include<grp.h>
#include<pwd.h>
#define PARAM_NONE 0
#define PARAM_A 1
#define PARAM_L 2
#define MAXROWLEN 80
int g_leave_len = MAXROWLEN;
int  g_maxlen;
void  my_err(const char *err_string,int line);
void  display_dir(int flag_param ,char *path);
void  display(int flag,char *pathname);
void   display_single(char *name);


int main(int argc,char *argv[])
{
    char path[PATH_MAX+1];
    char param[32];
    int flag_param = PARAM_NONE;
    struct stat      buf;
    //命令行参数的解析
    int j = 0, num = 0,i,k;
    for( i = 1; i < argc ; i ++ )
    {
        if( argv[i][0] == '-' )
        {
            for( k = 1; k < strlen(argv[i]) ; k++,j++)
            {
                param[j] = argv[i][k];              // 将 - 之后的字符存入数组中
            }
            num++;
        }
    }

    for( i = 0; i < j ; i ++ )
    {
        if(param[i] == 'a')
        {
            flag_param |= PARAM_A;
            continue;
        }
        else if(param[i] == 'l')
        {
            flag_param |= PARAM_L;
            continue;
        }
        else
        {
            printf("my_ls: invalid option -%c\n",param[i]);
            exit(1);
        }
    }
    param[j] = '\0';

    //如果没有输入文件名或者目录，就显示当前目录
    if((num+1) == argc)
    {
        strcpy(path,"./");
        path[2]='\0';
        display_dir(flag_param,path);
        return 0;
    }
    //my_ls -a -l    my_ls -a /bin
    i = 1;
    do
    {
        if(argv[i][0] == '-')
        {
            i++;
            continue;
        }
        else
        {
            strcpy(path,argv[i]);
            if(stat(path,&buf) == -1)
            {
                my_err("stat",__LINE__);
            }
            if(S_ISDIR(buf.st_mode))
            {
                //如果argv[i]是一个目录，且目录的最后一个字符不是'/'，就加上'/'
                if(path[strlen(argv[i]-1)] != '/')
                {
                    path[strlen(argv[i])] = '/';
                    path[strlen(argv[i])+1] = '\0';
                }
                else
                {
                    path[strlen(argv[i])] = '\0';
                }
                display_dir(flag_param,path);
                i++;
            }
            else
            {
                display(flag_param,path);
                i++;
            }
        }
    }while( i < argc );
    return 0;
}


//自定义错误处理函数
void  my_err(const char *err_string,int line)  
{
    fprintf(stderr,"line : %d",line);
    perror(err_string);
    exit(1);
}

void  display_dir(int flag_param ,char *path)
{
    DIR     *dir;
    int count = 0;
    struct dirent   *ptr;
    char filename[256][PATH_MAX+1],temp[PATH_MAX+1];
    //获取该目录下文件总数和最长的文件名
    dir = opendir (path);
    if(dir == NULL)
    {
        my_err("opendir",__LINE__);
    }
    while((ptr = readdir(dir)) != NULL)
    {
        if(g_maxlen < strlen(ptr->d_name))
            g_maxlen = strlen(ptr->d_name);
        count++;
    }
    closedir(dir);
    if( count>256 )
    {
        my_err("too many files under this dir",__LINE__);
    }

    //获取该目录下的所有文件名
    int i;
    int len = strlen(path);
    dir = opendir(path);
    for( i = 0 ; i < count ; i++)
    {
        ptr = readdir (dir);
        if(ptr == NULL)
            my_err ("readdir",__LINE__);
        strncpy(filename[i],path,len);
        filename[i][len] = '\0';
        strcat(filename[i],ptr->d_name);
        filename[i][len+strlen(ptr->d_name)] = '\0';
    }
    //冒泡排序法对文件名进行排序
    int j;
    for(i = 0; i < count-1 ; i++)
    {
        for(j = 0;j < count-1-i; j++)
        {
            if(strcmp(filename[j],filename[j+1]) > 0)
            {
                strcpy(temp,filename[j+1]);
                temp[strlen(filename[j+1])] = '\0';
                strcpy(filename[j+1],filename[j]);
                filename[j+1][strlen(filename[j])] = '\0';
                strcpy(filename[j],temp);
                filename[j][strlen(temp)] = '\0';
            }
        }
    }
    for( i =0 ;i < count ; i++ )
    {
        display(flag_param,filename[i]);
    }
    closedir(dir);
    //如果命令行没有-l选项，打印一个换行符
    if( (flag_param & PARAM_L) == 0)
    {
        printf("\n");
    }
}

void  display_attribute(struct stat buf,char *name)
{
    char  buf_time[32];
    struct passwd *psd;           //文件所有者的用户名
    struct group  *grp;           //文件所有者所属组的组名
    //获取并打印文件类型
    if( S_ISLNK(buf.st_mode))      //判断是否为符号链接
    {
        printf("l");
    }
    else if( S_ISREG(buf.st_mode) ) //判断是否为一般文件
    {
        printf("-");
    }
    else if( S_ISDIR(buf.st_mode) ) //判断是否为目录文件
    {
        printf("d");
    }
    else if(S_ISCHR(buf.st_mode))  //判断是否为字符设备文件
    {
        printf("c");
    }
    else if(S_ISBLK(buf.st_mode)) //判断是否为块设备文件
    {
        printf("b");
    }
    else if(S_ISFIFO(buf.st_mode)) //判断是否为先进先出FIFO
    {
        printf("f");
    }
    else if(S_ISSOCK(buf.st_mode)) //判断是否为socket
    {
        printf("s");
    }
    //获取并打印文件所有者的权限
    if(buf.st_mode & S_IRUSR)
    {
        printf("r");
    }

    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWUSR)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXUSR)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    //获取并打印文件所有者同组的用户的权限
    if(buf.st_mode & S_IRGRP)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWGRP)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXGRP)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    //获取并打印其他用户对该文件的操作权限
    if(buf.st_mode & S_IROTH)
    {
        printf("r");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IWOTH)
    {
        printf("w");
    }
    else
    {
        printf("-");
    }
    if(buf.st_mode & S_IXOTH)
    {
        printf("x");
    }
    else
    {
        printf("-");
    }
    printf("   ");

    //根据uid和gid获取文件所有者的用户名和组名
    psd = getpwuid(buf.st_uid);
    grp = getgrgid(buf.st_gid);
    printf("%4d ",buf.st_nlink);  //文件的链接数
    printf("%-8s",psd->pw_name);
    printf("%-8s",grp->gr_name);
    printf("%6d",buf.st_size);
    strcpy(buf_time,ctime(&buf.st_mtime)); //ctime()函数返回的字符串自带\n
    buf_time[strlen(buf_time)-1] = '\0';
    printf("    %s",buf_time);
}

//没有使用-l选项时，打印一个文件名，打印时上下对齐
void   display_single(char *name)
{
    if(g_leave_len < g_maxlen)
    {
        printf("\n");
        g_leave_len = MAXROWLEN;
    }
    int len;
    len = strlen(name);
    len = g_maxlen - len;
    printf("%-s",name);
    int i;
    for(i = 0; i < len ; i++ )
    {
        printf(" ");
    }
    printf(" ");
    g_leave_len -= (g_maxlen + 2);
}

void  display(int flag,char *pathname)
{
    struct stat    buf;
    char name[NAME_MAX + 1];
    //从路径中解析出文件名
    int i,j;
    for(i = 0,j = 0;i < strlen(pathname); i++)
    {
        if(pathname[i] == '/')
        {
            j = 0;
            continue;
        }
        name[j++] = pathname[i];
    }
    name[j] = '\0';
    //用lstat而不是stat以方便解析链接文件
    if(lstat(pathname, &buf) == -1)
        my_err("lstat",__LINE__);
    switch(flag)
    {
        case PARAM_NONE :
            {
                if(name[0] != '-')
                {
                    display_single (name);
                }
                break;
            }
        case PARAM_A :
            {
                display_single(name);
                break;
            }
        case PARAM_L:
            {
                if(name[0] != '-')
                {
                    display_attribute (buf,name);
                    printf("  %-s\n",name);
                }
                break;
            }
        case PARAM_A + PARAM_L :
            {
                display_attribute(buf,name);
                printf("  %-s\n", name);
                break;
            }
        default:
            break;
    }
}
