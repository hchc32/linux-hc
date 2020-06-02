// select如果在循环中，则时间的设置应该也放在循环中
#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<time.h>
#include<sys/time.h>

void display_time(const char *string)
{
    int seconds;
    seconds = time(NULL);
    printf("%s , %d\n",string,seconds);
}
int main()
{
    fd_set readfds;
    struct timeval time;
    
    //监视文件描述符0,是否又数据输入
    //清空文件描述符集合
    FD_ZERO(&readfds);
    FD_SET(0,&readfds);

    //设置阻塞时间为10s
  //  time.tv_sec = 10;
  //  time.tv_usec = 0;

    while(1)
    {
        time.tv_sec = 10;
        time.tv_usec = 0;
        display_time("before select");
        int ret = select(1,&readfds,NULL,NULL,&time);
        display_time("after select");
        if(ret == 0)
        {
            printf("No datat in ten seconds.\n");
            exit(0);
        }
        else if(ret == -1)
        {
            perror("select");
            exit(-1);
        }
        else
        {
            getchar();
            printf("Data is available now,.\n");
        }
        //getchar();
    }
    return 0;
}

