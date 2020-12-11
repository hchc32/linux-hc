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
    //初始化
    if(argc <= 2)
    {
        printf("usage: %s ip_address port backlog\n",basename(argv[0]));
        exit(1);
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    //int backlog = atoi(argv[3]);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    //assert(sock >= 0);
    printf("%d ,%d:%s\n",__LINE__,errno,strerror(errno));
    
    struct sockaddr_in address;
    bzero(&address , sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);
    if(bind(sock,(struct sockaddr*)&address,sizeof(address)) < 0)
    {
        printf("bind error! line:%d\n",__LINE__);
        exit(0);
    }
    if(listen(sock,5) < 0)
    {
        printf("listen error!line:%d\n",__LINE__);
        exit(0);
    }
    
    struct sockaddr_in client;
    socklen_t clilen = sizeof(client);
    int conn_fd;
    if((conn_fd = accept(sock,(struct sockaddr*)&client,&clilen)) < 0)
    {
        printf("accept error! line:%d\n",__LINE__);
        exit(0);
    }
    char buffer[BUF_SIZE];
    memset(buffer,'\0',sizeof(buffer));
    int ret = recv(conn_fd,buffer,BUF_SIZE-1,0);
    printf("got %d byte of normal data '%s' \n",ret , buffer);

    memset(buffer,'\0',sizeof(buffer));
    ret = recv(conn_fd,buffer,BUF_SIZE-1,0);
    printf("got %d byte of oob data '%s' \n",ret , buffer);
    
    memset(buffer,'\0',sizeof(buffer));
    ret = recv(conn_fd,buffer,BUF_SIZE-1,0);
    printf("got %d byte of normal data '%s' \n",ret , buffer);
    close(conn_fd);
    
    close(sock);
    return 0;
}
