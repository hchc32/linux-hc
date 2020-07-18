#include <stdio.h>
#include<sys/epoll.h>
#include<unistd.h>


int main(int argc,char* argv[])
{
    int epfd,count;
    struct epoll_event ev,events[5];

    epfd = epoll_create(1);
    ev.data.fd = STDIN_FILENO;
    //监听读状态同时设置ＥＴ模式
    ev.events = EPOLLIN | EPOLLET;
    
    //注册epoll事件
    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);
    while(1)
    {
        count = epoll_wait(epfd,events,5,-1);
        for(int i = 0 ; i < count ; i++)
        {
            if(events[i].data.fd == STDIN_FILENO)
            {
                printf("welcome to epoll's word!\n");
            }
        }

    }
    return 0;
}

