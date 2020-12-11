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

int main(int argc , char *argv[])
{
    //初始化
    if(argc <= 2)
    {
        printf("usage: %s ip_server_address port backlog\n",basename(argv[0]));
        exit(1);
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    //int backlog = atoi(argv[3]);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    //assert(sock >= 0);
    printf("%d ,%d:%s\n",__LINE__,errno,strerror(errno));
    
    struct sockaddr_in server_address;
    bzero(&server_address , sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port = htons(port);

    if(connect(sock,(struct sockaddr*)&server_address,sizeof(server_address)) < 0)
    {
       printf("connect error! line:%d\n",__LINE__);
       exit(0);
    }
    else
    {
        const char *oob_data = "abc";
        const char *normal_data = "123";
        send(sock,normal_data,strlen(normal_data),0);
        //发送带外数据
        send(sock,oob_data,strlen(oob_data),MSG_OOB);
        send(sock,normal_data,strlen(normal_data),0);
    }
    close(sock);
    return 0;
}
