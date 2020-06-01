#include <stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
int main()
{
    //定义一个套接字变量
    int sock_fd;
    //创建一个TCP套接字
    //AF_INET  指定套接字所用的协议族为IPV4 
    //SOCK_STREAM 指定套接字的类型为TCP流套接字
    //0表示通过前两个来确定使用的协议
   // sock_fd = socket(AF_INET, SOCK_STREAM , 0);
    //创建UDP协议
    sock_fd = socket(AF_INET,SOCK_DGRAM ,0);
    if(sock_fd < 0)
    {
        perror("socket");
        exit(1);
    }
    printf("%d\n",sock_fd);
    return 0;
}

