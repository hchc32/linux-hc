#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<wait.h>
#include<signal.h>
#include<readline/readline.h>
#include<readline/history.h>

#define normal       0 //一般的输出命令
#define out_redirect 1 //输出重定向
#define in_redirect  2 //输入重定向
#define have_pipe    3 //管道
#define SHELL_COL    "\033[1;34m"

void print_prompt();        
void get_input(char *buf);  //获取输入的命令字符串
void explain_input(char *buf, int *argcount , char arglist[100][256]); //解释命令字符串
int find_command (char *command);   //寻找命令是否存在
void do_cmd(int argcount ,char arglist[100][256]);
int cd_command(char* arg[2]);   //cd命令
void ShowHistory();             //history命令


int main(int argc ,char *argv[])
{
    sigset_t sig;
    int i;
    int argcount = 0;
    char *buf = NULL;
    char arglist[100][256];
    //存放键盘输入的字符串
    buf = (char *)malloc(256);
    
    //屏蔽信号ctrl+c
    sigemptyset(&sig);
    sigaddset(&sig,SIGINT);
    sigprocmask(SIG_BLOCK,&sig,NULL);
    
    if(buf == NULL)
    {
        printf("malloc filed ! line : %d\n",__LINE__);
        exit(-1);
    }
    while(1)
    {
        memset(buf,'\0',256);
        print_prompt();
       //获区键盘输入
       get_input(buf);
       //如果输入的命令是exit或logout则退出
       if(strcmp(buf,"exit\n") == 0 || strcmp(buf,"logout\n") == 0)
       {
           break;
       }
       for(int i = 0; i < 100; i++)
       {
           arglist[i][0] = '\0';
       }
       argcount = 0;
       explain_input(buf,&argcount,arglist);
       do_cmd(argcount,arglist);
    }

       if(buf != NULL)
       {
           free(buf);
           buf = NULL;
       }
    return 0;
}


void print_prompt()
{
    char buf[256];
    getcwd(buf,256);
    printf("\001\033[1m\002");
    printf("\033[34m");
    printf("\n\001my_shell$$ %s\002",buf);
    printf("\001\033[0m\002");
    memset(buf,'\0',256);
}

void get_input(char *buf)
{

    int len = 0;
    char ch = '\n';
    char* str;
    //不加字符串上下键寻找命令有问题
    str = readline(" : ");
    read_history("/tmp/my_histroy");
    add_history(str);
    write_history("/tmp/my_histroy");
    /*
    while(ch != '\n' && len < 256)
    {
        buf[len++] = ch;
        ch = getchar();
    }
    if(len == 256)
    {
        printf("command is too long ! len : %d\n",__LINE__);
        exit(-1);
    }
    buf[len] = '\n';
    len++;
    buf[len] = '\0';
    */
    strcpy(buf,str);
    free(str);
}

void explain_input(char *buf, int *argcount , char arglist[100][256])
{
    char *p = buf;
    char *q = buf;
    int number = 0;
    while(1)
    {
        if(p[0] == '\n')
            break;
        if(p[0] == ' ')
            p++;
        else
        {
            q = p;
            number = 0;
            while(q[0] != ' ' && q[0] != '\n')
            {
                q++;
                number++;
            }
            strncpy(arglist[*argcount],p,number);
            arglist[*argcount][number]  = '\0';
            *argcount += 1;
            p = q;
        }
    }
}

void do_cmd(int argcount ,char arglist[100][256])
{
    char *arg[argcount+1];     //存放每个命令
    int background = 0;        //后台运行的标志
    int flag = 0;              //是否有符号的标志
    int how = 0;               //符号的种类
    char *file;                //存储特殊符号后的字符
    char *argnext[argcount+1];
    //将命令取出
    printf("argcount%d\n",argcount);
    for(int i  = 0; i < argcount ; i++)
    {
        arg[i] = arglist[i];
    }
    arg[argcount] = NULL;
 /*   if(strcmp("cd",arg[0]) == 0)
    {
        if(chdir(arg[1]) < 0)
        {
            printf("cd %s error!\n",arg[1]);
            exit(0);
        }
        printf(" %s ",arg[1]);
        return;
    }
    */
    if( cd_command(arg) == 1)
    {
        return ;
    }
    if(strcmp(arg[0],"history") == 0)
    {
        ShowHistory();
        return;
    }
    //查看命令行是否有后台运行符
    for(int i = 0; i < argcount ; i++)
    {
        if(strncmp(arg[i],"&",1) == 0)
        {
            if(i == argcount-1)
            {
                background  = 1;
                arg[argcount-1] = NULL;
                break;
            }
            else
            {
                printf("wrong command ! line : %d\n",__LINE__);
                return ;
            }
        }
    }

    for(int i = 0; arg[i] != NULL; i++)
    {
        //是否有重定向输出符号
        if(strcmp(arg[i],">") == 0)
        {
            flag++;
            how = out_redirect; 
            if(arg[i+1] == NULL)
                flag++;
        }
        //是否有重定向输入符号
        if(strcmp(arg[i],"<") == 0)
        {
            flag++;
            how = in_redirect; 
            if(i == 0)
                flag++;
        }
        //是否有管道符号
        if(strcmp(arg[i],"|") == 0)
        {
            flag++;
            how = have_pipe; 
            if(arg[i+1] == NULL)
                flag++;
            if(i == 0)
                flag++;
        }

    }
    //flag > 1,说明命令格式不对,不支持多个符号
    if(flag > 1)
    {
        printf("wrong command ! line : %d\n",__LINE__);
        return ;
    }
    //命令中含有一个 > 
    if(how == out_redirect)
    {
        for(int i = 0;arg[i] != NULL ; i++)
        {
            if(strcmp(arg[i],">") == 0)
            {
                file = arg[i+1];
                arg[i] == NULL;
            }
        }
    }
    //命令中含有一个 <
    if(how == in_redirect)
    {
        for(int i = 0;arg[i] != NULL ; i++)
        {
            if(strcmp(arg[i],"<") == 0)
            {
                file = arg[i+1];
                arg[i] == NULL;
            }
        }

    }
    //命令中含有一个 |
    if(how == have_pipe)
    {
        for(int i = 0;arg[i] != NULL ; i++)
        {
            if(strcmp(arg[i],"|") == 0)
            {
                arg[i] = NULL;
                int j;
                for(j = i + 1; arg[j] != NULL ; j++)
                {
                    argnext[j-1-i] = arg[j];
                }
                argnext[j-1-i] = arg[j];
                break;
            }
        }
    }
    pid_t pid = fork();
    if(pid < 0)
    {
        printf("fork error ! line : %d\n",__LINE__);
        return ;
    }
    switch(how)
    {
        //一般命令
        case 0:
        {
            //是子进程
            if(pid == 0)
            {
                if(!find_command(arg[0]))
                {
                    printf("%s : command not found ! \n line : %d\n",arg[0],__LINE__);
                    exit(0);
                }
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        }
        //有 > 的命令        
        case 1:
        {
            int fd;
            if(pid == 0)
            {
                arg[1] = NULL;
                if(!find_command(arg[0]))
                {
                    printf("%s : command not found ! \n line : %d\n",arg[0],__LINE__);
                    exit(0);
                }
                fd = open(file,O_RDWR | O_CREAT | O_TRUNC,0644);
                dup2(fd,1);
                execvp(arg[0],arg);
                exit(0);
            }
            
            break;
        }
        //有 < 的命令
        case 2:
        {
            int fd;
            if(pid == 0)
            {
                if(!find_command(arg[0]))
                {
                    printf("%s : command not found ! \n line : %d\n",arg[0],__LINE__);
                    exit(0);
                }
                fd = open(file,O_RDONLY);
                dup2(fd,0);
                execvp(arg[0],arg);
                exit(0);
            }
            break;
        }
        //有 | 的命令
        case 3:
        {
            if(pid == 0)
            {
                int pid2;
                int fd2;
                if((pid2 = fork()) < 0)
                {
                    printf("fork2 error\n");
                    return ;
                }
                else if(pid2 == 0)
                {
                    if( !(find_command(arg[0])) )
                    {
                        printf("%s : command not found ! \n line : %d\n",arg[0],__LINE__);
                        exit(0);
                    }
                    fd2 = open("/tmp/youdonotknowfile",O_WRONLY | O_CREAT | O_TRUNC,0644);
                    dup2(fd2,1);
                    execvp(arg[0],arg);
                    exit(0);
                }
                else
                {
                    //父进程逻辑
                    int status2;
                    if( waitpid(pid2 , &status2 , 0) == -1 )
                    {
                        printf("wait for child process error! line : %d\n",__LINE__);
                    }
                    if( !(find_command(argnext[0])) )
                    {
                        printf("%s : command not found ! \n line : %d\n",argnext[0],__LINE__);
                        exit(0);
                    }
                    fd2 = open("/tmp/youdonotknowfile",O_RDONLY);
                    dup2(fd2,0);
                    execvp(argnext[0],argnext);
                    //删除文件
                    if(remove("/tmp/youdonotknowfile"))
                    {
                        printf("remove file error ! line : %d\n",__LINE__);  
                    }
                    exit(0);
                }
            }
            break;
        }
        default: 
            break;
    }
    //如果命令中又&,表示后台执行,父进程直接返回,不等待子进程结束
    if(background == 1)
    {
        printf("process id %d\n",pid);
        return;
    }
    int status;
    if(waitpid(pid, &status,0) == -1)
    {
        printf("wait for child process error\n");

    }
}

int find_command (char *command)
{
    DIR* dp;
    struct dirent* dirp;
    char* path[] = {"./", "/bin", "/usr/bin","/usr/local/bin","/usr/sbin",NULL};
    //当前目录下的命令也可以运行
    if(strncmp(command,"./",2) == 0)
    {
        command += 2;
    }
    int i = 0;
    while(path[i] != NULL)
    {
        if( (dp = opendir(path[i])) == NULL)
            printf("can not open the dir ! line : %d\n",__LINE__);
        while((dirp = readdir(dp)) != NULL)
        {
            if(strcmp(dirp->d_name,command) == 0)
            {
                closedir(dp);
                return 1;
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}

int cd_command(char* arg[2])
{
    if(strcmp(arg[0],"cd") == 0)
    {
        if(chdir(arg[1]) < 0)
        {
            printf("cd %s error! line: %d\n",arg[1],__LINE__);
        }
        printf("%s %s : ",arg[0],arg[1]);
        return 1;
    }
    return 0; 
}

void ShowHistory()
{
    int i = 0;
    HIST_ENTRY ** his;
    his = history_list();
    while(his[i] != NULL)
    {
        printf("%s\n", his[i]->line);
        i++;
    }
}
