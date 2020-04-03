#include <stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<wait.h>
int main()
{
    //打开一个文件
    int fd = open("test.txt",O_RDWR);
    //获取文件大小
    int len = lseek(fd,0,SEEK_END);
    void* ptr = mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }
    close(fd);
    //创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        exit(1); 
    }
    if(pid > 0)//父进程写数据
    {
        sleep(3);
        //写数据
        strcpy((char*)ptr,"hello world!");
        //回收
        wait(NULL);
    }
    else if(pid == 0)
    {
        //读数据
        printf("%s\n",(char*)ptr);
    }


    //释放内存映射区
    int ret = munmap(ptr,len);
    if(ret == -1)
    {
        perror("munmap");
        exit(1);
    }
    return 0;
}

