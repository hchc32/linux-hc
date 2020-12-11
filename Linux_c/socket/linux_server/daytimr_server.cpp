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
#include<fstream>
#include<assert.h>
#include<signal.h>
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
#include<iostream>
#include<string>
#include<fstream>
#include<netdb.h>
using namespace std;

#define BUF_SIZE 1024
int main(int argc , char *argv[])
{
    
    if( argc >= 1 )
    {
        printf("example : ./a.out ip_address!\n");
        exit(0);
    }
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    int ans = bind(sockfd,(struct sockaddr*)&address,sizeof(address));
    if(ans < 0)
    {
        printf("bind error!line:%d\n",__LINE__);
        exit(0);
    }
    listen(sockfd,2);
    int len = sizeof(address);
    while(1)
    {
        if(accept(sockfd,(struct sockaddr*)&address,(socklen_t*)&len) < 0)
        {
           printf("%d,%s\n",errno,strerror(errno));
           printf("connect error!line:%d\n",__LINE__);
           exit(0);
        }

        char buffer[BUF_SIZE];
        printf("connect with ip: %s and port: %d\n",\
               inet_ntop(AF_INET,&address.sin_addr,buffer,sizeof(buffer))\
               ,ntohs(address.sin_port));
        int ans;
        if(( ans = read(sockfd,buffer,sizeof(buffer))) < 0)
        {
            printf("read error! line:%d\n",__LINE__);
            exit(0);
        }

        buffer[ans] = '\0';
        printf("the daytime tiem is : %s",buffer);
        close(sockfd);
    }
    return 0;
}
