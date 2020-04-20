#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#define normal       0 //一般的输出命令
#define out_redirect 1 //输出重定向
#define in_redirect  2 //输入重定向
#define have_pipe    3 //管道
void print_prompt();
void get_input(char *buf);
void explain_input(char *buf, int *argcount , char arglist[100][256]);
int find_command (char *command);
int main(int argc ,char *argv[])
{
    int i;
    int argcount = 0;
    char *buf = NULL;
    buf = (char *)malloc(256);
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
    }
    return 0;
}

void print_prompt()
{
    printf("myshell$$ ");
}

void get_input(char *buf)
{
    int len = 0;
    int ch;
    ch = getchar();
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
    char *arg[argcount+1];
    int background = 0;
    int flag = 0;
    int how = 0;
    
    //将命令取出
    for(int i  = 0; i < argcount ; i++)
    {
        arg[i] = arglist[i];
    }
    arg[argcount] = NULL;

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

    //
    for(int i = 0; arg[i] != NULL; i++)
    {
        if(strcmp(arg[i],">") == 0)
        {
            flag++;
            how = out_redirect; 
            if(arg[i+1] == NULL)
                flag++;
        }
        if(strcmp(arg[i],"<") == 0)
        {
            flag++;
            how = in_redirect; 
            if(i == 0)
                flag++;
        }
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
    //flag > 1,说明命令格式不对 
    if(flag > 1)
    {
        printf("wrong command ! line : %d\n",__LINE__);
        return ;
    }
    //命令中含有一个 > 
    if(how == out_redirect)
    {
        
    }
}

int find_command (char *command)
{
    DIR* dp;
    struct dirent* dirp;
    char* path[] = {"./", "/bin", "/usr/bin", NULL};
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
