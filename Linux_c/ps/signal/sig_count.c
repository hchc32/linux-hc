#include <stdio.h>
#include<unistd.h>
#include<signal.h>
#include<fcntl.h>
#include<stdlib.h>
int flag = 0;
int count = 0;
pid_t pid;
void sig_count_father(int sig)
{
    sleep(1);//必须有,不然会不稳定,会使得flag恒为0
    printf("%d\n",count);
    flag = 1; 
    count += 2;
}
void sig_count_son(int sig)
{
    sleep(1);
    printf("%d\n",count);
    flag = 1; 
    count += 2;
}
int main()
{
    pid = fork();
    if(pid == 0)// 子进程
    {
        count=1;
        signal(SIGUSR2,sig_count_son);
        pid_t ppid = getppid();
        while(1)
        {
            if(flag == 1)
            {
                kill(ppid,SIGUSR1);
                flag = 0;
            }
        }
    }
    else if(pid > 0)//父进程 
    {
        usleep(10);
        count = 2;
        signal(SIGUSR1 , sig_count_father);
        kill(pid,SIGUSR2);
        while(1)
        {
            if(flag == 1)
            {
                kill(pid,SIGUSR2);
                flag = 0;
            }
        }
    }
    return 0;
}

