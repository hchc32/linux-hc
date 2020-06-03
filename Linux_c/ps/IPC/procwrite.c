#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<string.h>

#define FIFO_NAME "myfifo"
#define BUF_SIZE 1024

int main()
{
    int fd;
    char buf[BUF_SIZE] = "Hello procwrite, I come from process named procread";
    umask(0);
    if(mkfifo(FIFO_NAME , S_IFIFO | 0666) == -1)
    {
        perror("mkfifo error!");
        exit(-1);
    }
    //以写的方式打开FIFO
    if((fd = open(FIFO_NAME , O_WRONLY)) == -1)
    {
        perror("open error!");
        exit(-1);
    }
    //向fifo中写数据
    write(fd,buf,strlen(buf));
    close(fd);

    return 0;
}

