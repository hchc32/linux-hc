#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
    //最后一个参数是非阻塞方式打开文件
    //tty输入输出设备
    int fd = open("/dev/tty",O_RDWR|O_NONBLOCK);
    if(fd == -1)
    {
        printf("file open error!\n");
        return -1;
    }
    char buf[256];
    int ret = 0;
    while(1)
    {
        //此处发生阻塞
        ret = read(fd,buf,sizeof(buf));
        if(ret < 0)
        {
            printf("read file error!\n");
        }
        if( ret )
        {
            printf("buf is %d\n",ret);
        }
        printf("hello!\n");
        sleep(1);
    }
    close(fd);
    return 0;
}

