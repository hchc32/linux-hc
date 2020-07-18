#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<signal.h>
#include<wait.h>
#include<pthread.h>
#include"my_recv.h"


typedef struct Sock
{
    int fd;
    struct sockaddr_in addr;
    pthread_t id;
}Sock;

void* worker(void* arg)
{
    Sock* info = (Sock*)arg;
    char ip[32];
    char buf[1024];
    //进行通信
    while(1)
    {
        printf("client ip: %s,port: %d\n",
               inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip)),
               ntohs(info->addr.sin_port));

        int len = read(info->fd,buf,sizeof(buf));
        if(len == -1)
        {
            printf("raed error!\n");
            pthread_exit(NULL);
        }
        else if(len == 0)
        {
            printf("客户端已经断开连接\n");
            close(info->fd);
            break;
        }
        else
        {
            printf("recv buf: %s\n",buf);
            write(info->fd,buf,len);
        }
    }
    return NULL;
}


int main(int argc,char* argv[])
{
    if(argc < 2)
    {
        printf("./a.out port\n");
        exit(1);
    }

    struct sockaddr_in serv_addr;
    int port = atoi(argv[1]);
    //创建套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    //初始化serv_addr
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定iP和端口
    bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    //设置监听的最大个数
    listen(lfd,36);
    printf("Start accept ......\n");

    int i = 0;
    Sock info[256];
    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(struct sockaddr_in);
    while(1)
    {
        //主线程等待接受连接请求
        info[i].fd = accept(lfd,(struct sockaddr*)&info[i].addr,&cli_len);
        
        //创建子线程
        pthread_create(&info[i].id,NULL,worker,&info[i]);
        
        //设置线程分离,不用回收子线程
        pthread_detach(info[i].id);
        i++;
        if(i >= 256)
        {
            break;
        }
        

    }
    close(lfd);

    //只退出主线程
    pthread_exit(NULL);

    return 0;
}

