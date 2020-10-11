#include <stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<error.h>
#include<string.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("example : ip_addr port\n");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    //创建socket套接字
    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);
    
    printf("errno:%d %s",errno,strerror(errno));
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    printf("errno:%d %s",errno,strerror(errno));
/*
    if(ret < 0)
    {
        printf("bind error!\n");
    }
    */
    
    assert(ret != -1);
    ret = listen(sock,5);
    assert(ret != -1);

    //sleep(20);
    printf("******************\n");
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
    int connfd = accept(sock,(struct sockaddr*)&client,&client_addrlength);
    printf("******************\n");
    if(connfd < 0)
    {
        printf("accept error !\n");
    }
    else
    {
        //接受连接成功则打印出客户端的ip地址和
        char remote[INET_ADDRSTRLEN];
        printf("%d\n",INET_ADDRSTRLEN);
        printf("connected with ip : %s and port : %d\n",\
               inet_ntop(AF_INET,&client.sin_addr,remote,INET_ADDRSTRLEN),\
               ntohs(client.sin_port));
        close(connfd);
    }
    return 0;
}

