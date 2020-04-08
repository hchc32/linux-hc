#include <stdio.h>
#include<sys/time.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
    //设置一个定时器
    struct itimerval new_val;
    //第一次触发的时间
    new_val.it_value.tv_sec = 2;
    new_val.it_value.tv_usec = 0;
    //周期性计时
    new_val.it_interval.tv_sec = 1;
    new_val.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL,&new_val ,NULL);
    while(1)
    {
        printf("hello,linux\n");
        sleep(1);
    }
    return 0;
}

