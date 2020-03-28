#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    int n = 5;
    int i = 0;
    pid_t pid = 0;
    for(i = 0; i < 5; i++)
    {//父进程循环结束
        pid = fork();
        if(pid == 0)
        {
            //son
            printf("I am child , pid  = %d, ppid = %d\n",getpid(),getppid());
            break;        //子进程推出循环的接口
        }
        else if(pid > 0)
        {
            //father
            //printf("I am father ,pid = %d, ppid = %d\n",getpid(),getppid()); 
        }
    }
    sleep(i);
    if(i < 5)
    {
        printf("I am child ,will exit,pid = %d,ppid = %d\n",getpid(),getppid());
    }
    else
    {
        printf("I am parent ,will out pid = %d,ppid = %d\n",getpid(),getppid());
    }
    return 0;
}

