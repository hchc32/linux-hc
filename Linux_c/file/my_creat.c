#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<errno.h>

int main()
{
    int fd;
    //文件所有者的信息 r w
   // if((fd = open("example_62.c",O_CREAT | O_EXCL ,S_IRUSR | S_IWUSR )) ==-1)
   // {
        if((fd = creat("example_62.c",S_IWUSR)) == -1){
        //perror("open");//creat的话，可以执行多次，因为如果文件存在，会覆盖原文件，但open不可以
        printf("open:%s    with errno:%d\n",strerror(errno),errno);
        //errno: 用来记录系统的最后一次错误代码
        exit(1);
    }
    else
    {
        printf("Creat file success\n");
    }
    close(fd);

    return 0;
}

