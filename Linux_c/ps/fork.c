#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    printf("Begin~~~~~~~~~\n");
    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork error\n");
        exit(1);
    }
    if(pid == 0)
    {
        // 是子进程
        printf("I am child ,pid = %d, ppid = %d\n",getpid(),getppid());
        while(1)
        {
            printf("I am a child\n");
            sleep(1);
        }
    }
    else if(pid > 0)
    {
        // 父进程的逻辑
        printf("childpid = %d, selfpid = %d,ppid = %d\n",pid,getpid(),getppid());
        while(1)
        {
            sleep(1);
        }
    }
    printf("End~~~~~~~~~~~\n");
    return 0;
}

