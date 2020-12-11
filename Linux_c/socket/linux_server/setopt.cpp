#include <stdio.h>             
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/mman.h>
#include<pthread.h>
#include<semaphore.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<errno.h>
//#include<iostream>
#include<string>
#include<fstream>
#include<assert.h>
#include<signal.h>

#define BUF_SIZE 1024


int main(int argc , char *argv[])
{
    if(argc <= 2)
    {
        printf("usage: %s ip_server_adderss port backlog\n",basename(argv[0]));
        exit(1);
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    //int backlog = atoi(argv[3]);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    //assert(sock >= 0);
    printf("%d ,%d:%s\n",__LINE__,errno,strerror(errno));
    
    struct sockaddr_in server_adderss;
    bzero(&server_adderss , sizeof(server_adderss));
    server_adderss.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_adderss.sin_addr);
    server_adderss.sin_port = htons(port);

    //修改ＴＣＰ发送缓冲区的大小
    int sendbuf = atoi(argv[3]);
    int len = sizeof(sendbuf);
    setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,sizeof(sendbuf));
    getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,(socklen_t*)&len);
    printf("the tcp send buffer size after setting is %d\n",sendbuf);
    
    if(connect(sock,(struct sockaddr*)&server_adderss,sizeof(server_adderss)) != -1)
    {
        char buffer[BUF_SIZE];
        memset(buffer,'a',sizeof(buffer));
        recv(sock,buffer,sizeof(buffer),0);
    }
    close(sock);

    close(sock);
    return 0;
}
