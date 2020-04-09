#include <stdio.h>
#include<unistd.h>
#include<signal.h>

void myfun(int no)
{
    printf("hello linux\n");
    sleep(3);
    printf("wake up\n");
}

int main()
{
    struct sigaction action;
    //初始化结构体
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    //添加临时屏蔽信号
    sigaddset(&action.sa_mask ,SIGQUIT);
    action.sa_handler = myfun;
    sigaction(SIGINT,&action,NULL);
    while(1);
    return 0;
}

