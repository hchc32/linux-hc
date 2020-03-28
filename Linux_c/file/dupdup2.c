//先将数据写入文件中，再打印在屏幕上
#include <stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>

int main()
{
    //备份
    int outfd = dup(1);
    //重定向
    int fd = open("world",O_WRONLY|O_CREAT,0666);
    dup2(fd,1);

    printf("hello\n");
    //printfY需要调用底层的write,只有刷新FILE*中的缓冲区，数据才可以写入
    fflush(stdout);
    //恢复1所对应的标准输出
    dup2(outfd,1);

    printf("hello\n");
    close(fd);
    return 0;
}

