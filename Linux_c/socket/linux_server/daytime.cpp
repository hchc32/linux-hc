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
#include<iostream>
#include<string>
#include<fstream>
#include<netdb.h>
using namespace std;

#define BUF_SIZE 1024
int main(int argc , char *argv[])
{
    
    if( argc !=2 )
    {
        printf("example : ./a.out ip_address!\n");
        exit(0);
    }
    char *host = argv[1];
    //根据主机名称获区目标主机地址信息
    struct hostent* hostinfo = gethostbyname(host);
    if(hostinfo == NULL )
    {
        printf("gethostbyname error ! line :%d\n",__LINE__);
        exit(0);
    }
    //获取daytime信息
    struct servent* servinfo = getservbyname("daytime","tcp");
    if(servinfo == NULL)
    {
        printf("getservbyname error!line : %d\n",__LINE__);
        exit(0);
    }
    printf("daytime port is %d\n",ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;
    //主机的ip地址
    address.sin_addr = *(struct in_addr*)*hostinfo->h_addr_list;

    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(connect(sockfd,(struct sockaddr*)&address,sizeof(address)) < 0)
    {
        printf("%d,%s\n",errno,strerror(errno));
       printf("connect error!line:%d\n",__LINE__);
       exit(0);
    }

    char buffer[BUF_SIZE];
    int ans;
    if(( ans = recv(sockfd,buffer,sizeof(buffer),0)) < 0)
    {
        printf("%d,%s\n",errno,strerror(errno));
        printf("read error! line:%d\n",__LINE__);
        exit(0);
    }

    buffer[ans] = '\0';
    printf("the daytime tiem is : %s",buffer);
    close(sockfd);
    return 0;
}
