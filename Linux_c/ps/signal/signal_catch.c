#include <stdio.h>
#include<signal.h>
#include<unistd.h>

void catch_signal(int no)
{
    printf("catch you signal: %d\n",no);
}

int main()
{
    //捕捉ctrl+c
    signal(SIGINT,catch_signal);
    while(1)
    {
        printf("linux\n");
        sleep(1);
    }
    //设置捕捉函数
    return 0;
}

