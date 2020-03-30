//非阻塞
#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
    //最后一个参数是非阻塞方式打开文件
    //打开当前的终端设备
    int fd = open("/dev/tty",O_RDWR|O_NONBLOCK);
    if(fd == -1)
    {
        printf("file open error!\n");
        return -1;
    }
    char buf[256];
    int ret = 0;
    while((ret = read(fd,buf,sizeof(buf))) < 0)
    {
            sleep(3);
            printf("read file error!\n");
    }
    if( ret )
    {
        sleep(1);
        printf("read file success ! buf is %d\n",ret);
    }
    close(fd);
    return 0;
}

