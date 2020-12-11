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

    int recvbuf = atoi(argv[3]);
    int len = sizeof(recvbuf);
    setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,sizeof(recvbuf));
    getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,(socklen_t*)&len);
    printf("the tcp recv buffer size after setting is %d\n",recvbuf);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    //assert(ret !=-1 );
    printf("%d ,%d:%s\n",__LINE__,errno,strerror(errno));

    ret = listen(sock,5);
    printf("%d:%s\n",errno,strerror(errno));
    //assert(ret >= 0);
    //sleep(1);
    //接受客户端连接的信息
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    //if(1)
    {
        int conn_fd = accept(sock,(struct sockaddr*)&client,&client_addrlength);
        printf("%d:%s\n",errno,strerror(errno));
        if(conn_fd < 0)
        {
            printf("accept errno!\n");
        }
        else
        {
            //接受客户端的信息
            char remote[INET_ADDRSTRLEN];
            printf("connect with ip: %s and port: %d\n",\
                   inet_ntop(AF_INET,&client.sin_addr,remote,INET_ADDRSTRLEN),ntohs(client.sin_port));
            char buffer[BUF_SIZE];
            memset(buffer,'\0',sizeof(buffer));
            while(recv(conn_fd,buffer,sizeof(buffer),0) > 0)
            {
            }

            close(conn_fd);
        }
    }
    close(sock);
    return 0;
}
