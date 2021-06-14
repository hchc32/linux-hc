#include<iostream>
#include<unistd.h>
#include<sys/timerfd.h>
#include<sys/select.h>
#include<sys/types.h>
#include<sys/time.h>
#include<cstring>

using namespace std;

/*
表示时间
struct timespec
{
    time_t tv_sec; //秒
    long tv_nsec;  //毫秒
}
设置定时器的工作方式
struct itimerspec
{
    struct timespec it_interval; //超时间隔时间
    struct timespec it_value;    //第一次超时的时间
}
*/

int main()
{
    //定时器fd
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC,TFD_NONBLOCK|TFD_CLOEXEC);

    //设置定时器参数
    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 3;
    timerfd_settime(timerfd,0,&howlong,nullptr);

    //select监听timerfd
    fd_set rdset;
    FD_ZERO(&rdset);
    FD_SET(timerfd, &rdset);
    
    //指定select要等待的时间
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    while(1)
    {
        //select等待超时返回0
        if(select(timerfd+1,&rdset,NULL,NULL,&timeout) == 0)
        {
            cout << "timeout" << endl;
            //select返回后会把以前加入但是没有事件发生的fd清空掉
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;
            FD_SET(timerfd, &rdset);
            continue;
        }
        cout << "timer happend" << endl;
        break;
    }

    //关闭文件描述符
    ::close(timerfd);
    return 0;
}