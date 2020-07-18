#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<signal.h>
#include<wait.h>
#include"my_recv.h"

void recyle(int num)
{
    pid_t pid;
    while((pid = waitpid(-1,NULL,WNOHANG)) > 0)
    {
        printf("child died , pid = %d\n",pid);
    }
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
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    //绑定ＩＰ和端口
    bind(lfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    //设置同时监听的最大个数
    listen(lfd,36);
    printf("Start accept ......\n");

    //使用信号回收子进程PCB
    struct sigaction act;
    act.sa_handler = recyle;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD,&act,NULL);
    struct sockaddr_in client_addr;
    socklen_t cli_len = sizeof(client_addr);

    while(1)
    {
        //父进程接受连接请求
        int cfd = accept(lfd,(struct sockaddr*)&client_addr,&cli_len);
        while(cfd == -1)
        {
            cfd = accept(lfd,(struct sockaddr*)&client_addr,&cli_len);
        }
        printf("connect success!");
        //创建子进程
        pid_t pid = fork();
        //子进程
        if(pid == 0)
        {
            close(lfd);
            //通信
            char ip[64];
            while(1)
            {
                printf("client IP: %s, port: %d\n",\
                       inet_ntop(AF_INET,&client_addr.sin_addr.s_addr,ip,sizeof(ip)),
                       ntohs(client_addr.sin_port));
                char buf[1024];
                int len = read(cfd,buf,sizeof(buf));
                if(len == -1)
                {
                    my_err("read",__LINE__);
                    exit(1);
                }
                else if(len == 0)
                {
                    printf("客户端断开了连接\n");
                    close(cfd);
                    break;
                }
                else
                {
                    printf("recv buf: %s\n",buf);
                    write(cfd,buf,len);
                }
            }
            //杀死子进程
            return 0;
        }
    }
    close(lfd);
    return 0;
}

