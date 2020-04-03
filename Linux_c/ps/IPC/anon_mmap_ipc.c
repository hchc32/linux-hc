#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<errno.h>
#include<wait.h>
int main(int argc,char* argv[])
{
    //创建匿名内存映射区
    int len = 4096;
    //文件描述符为-1,加上宏MAP_ANON
    void* ptr = mmap(NULL, len ,PROT_READ | PROT_WRITE ,\
                    MAP_SHARED | MAP_ANON,-1,0);
    //创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork");
        exit(1);
    }
    if(pid > 0) //父进程
    {
        //写数据
        strcpy((char*)ptr,"xiyou linux group");
        wait(NULL);
    }
    else if(pid == 0 )//子进程
    {
        printf("%s\n",(char*)ptr);
    }
    int ret = munmap(ptr,len);
    if(ret == -1)
    {
        perror("munmap");
        exit(1);
    }
    return 0;
}

