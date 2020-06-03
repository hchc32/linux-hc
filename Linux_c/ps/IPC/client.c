#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>

#define FIFO_READ "writefifo"
#define FIFO_WRITE "readfifo"
#define BUF_SIZE 1024


int main()
{
    int wfd,rfd;
    char buf[BUF_SIZE];
    int len;
    umask(0);
    if(mkfifo(FIFO_WRITE,S_IFIFO|0666))
    {
        printf("can not creat FIFO %s because %s",FIFO_WRITE,strerror(errno));
        exit(1);
    }
    while((rfd = open(FIFO_READ,O_RDONLY)) == -1)
    {
        sleep(1);
    }
    wfd = open(FIFO_WRITE,O_WRONLY);
    if(wfd == -1)
    {
        printf("open FIFO %s error : %s",FIFO_WRITE,strerror(errno));
        exit(1);
    }

    while(1)
    {
        len = read(rfd,buf,BUF_SIZE);
        if(len > 0)
        {
            buf[len] = '\0';
            printf("Server: %s\n",buf);
        }
        printf("Client:");
        fgets(buf , BUF_SIZE , stdin);
        buf[sizeof(buf)-1] = '\0';
        if(strncmp(buf,"quit",4) == 0)
        {
            close(wfd);
            unlink(FIFO_WRITE);
            close(rfd);
            exit(0);
        }
        write(wfd,buf,sizeof(buf));

        /*

        */
    }
    return 0;
}

