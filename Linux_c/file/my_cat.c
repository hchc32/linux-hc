#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
char buf[256];
int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("./a.out filenmae");
        return -1;
    }
    int fd = open(argv[1],O_RDONLY);
    int ret  = read(fd,buf,sizeof(buf));
    if(ret == -1)
    {
        printf("file read error");
        return -1;
    }
    else//输入输出到屏幕上
    {
        write(STDOUT_FILENO,buf,ret);
    }
    close(fd);
    return 0;
}

