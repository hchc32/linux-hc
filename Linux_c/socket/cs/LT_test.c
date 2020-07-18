#include <stdio.h>
#include<unistd.h>
#include<sys/epoll.h>

int main()
{
    int epfd,count;
    struct epoll_event even,events[5];
    epfd = epoll_create(1);

    even.data.fd = STDIN_FILENO;
    even.events = EPOLLIN ;

    epoll_ctl(epfd,EPOLL_CTL_ADD,STDIN_FILENO,&even);

    while(1)
    {
        count = epoll_wait(epfd,events,5,-1);
        for(int i =0 ;i < count; i++)
        {
            if(events[i].data.fd == STDIN_FILENO)
            {
                printf("welcome to epoll's word!\n");
            }
        }
    }

    
    return 0;
}
