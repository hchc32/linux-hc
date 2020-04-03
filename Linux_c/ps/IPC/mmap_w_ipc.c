//无血缘关系的mmap进程间通信
#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<sys/mman.h>
int main()
{
    //创建文件
    int fd = open("temp1",O_CREAT | O_RDWR,0664 );
    //拓展文件大小
    ftruncate(fd,4096);
    int len = lseek(fd,0,SEEK_END);
    //创建内存映射区
    void* ptr = mmap(NULL,len,PROT_READ | PROT_WRITE ,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    //进行读操作
    while(1)
    {
        sleep(1);
        printf("%s\n",(char*)ptr);
    }
    //释放映射区
    int ret = munmap(ptr,len);
    if(ret == -1)
    {
        perror("munmap");
        exit(1);
    }
    return 0;
}
