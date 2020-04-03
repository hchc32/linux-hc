#include <stdio.h>
#include<sys/mman.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
int main()
{
    int fd = open("test.txt",O_RDWR);
    //获取文件大小
    int len = lseek(fd ,0 ,SEEK_END);
   //创建内存映射区
   void* ptr = mmap(NULL, len , PROT_READ | PROT_WRITE,MAP_SHARED,fd ,0);
   //ptr是指向映射区的首地址
   if(ptr == MAP_FAILED)
   {
        perror("mmap");
        exit(1);
   }
   //将内存映射区的数据打印到终端上
   printf("%s",(char*)ptr);
   //释放映射区
   munmap(ptr,len);
   close(fd);
   return 0;
}

