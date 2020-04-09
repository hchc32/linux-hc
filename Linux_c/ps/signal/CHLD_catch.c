#include <stdio.h>
#include<wait.h>
#include<signal.h>
#include<unistd.h>

void catch_sig(int num)
{
    pid_t wpid;
    while((wpid = waitpid(-1,NULL,WNOHANG)) > 0)
    {//多个信号同时发出,不会每个信号处理一次
        if(wpid > 0)
        {
            printf("wait child %d ok\n",wpid);
        }
    }
}

int main()
{
    //创建子进程之前先屏蔽SIGCHLD信号
    sigset_t  myset,oldset;
    sigemptyset(&myset);
    sigaddset(&myset,SIGCHLD);
    //old用来保留之前的信号集状态
    sigprocmask(SIG_BLOCK,&myset,&oldset);
    int i = 0;
    pid_t pid;
    for(i = 0; i < 10 ; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }
    if(i == 10)//父进程
    {
        sleep(2);
        struct sigaction act;
        act.sa_flags = 0;
        sigemptyset(&act.sa_mask);
        act.sa_handler = catch_sig;//回调函数
        sigaction(SIGCHLD,&act,NULL);
        //解除屏蔽
        sigprocmask(SIG_SETMASK,&oldset,NULL);
        while(1)
        {
            sleep(1);
        }
    }
    else if(pid < 10)//子进程
    {
        printf("I am %d child , pid = %d\n",i,getpid());
        //sleep(i);
    }
    return 0;
}

