#include <stdio.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<ctype.h>
#include<sys/socket.h>
#include<string.h>
#include<stdlib.h>

int main(int argc,char* argv[])
{
    if(argc < 2)
    {
        printf("./a.out prot\n");
        exit(1);
    }
    
    int sfd,port;
    struct sockaddr_in serv_addr;
    //创建一个套接字
    sfd = socket(AF_INET,SOCK_STREAM,0);
    //设置
    port = atoi(argv[1]);
    socklen_t serv_len = sizeof(serv_addr);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定
    bind(sfd,(struct sockaddr*)&serv_addr,serv_len);
    //监听
    listen(sfd,2);
    printf("Start accept ......\n");
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int epfd = epoll_create(2000);
    
    //初始化epoll树
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);

    struct epoll_event all[2000];
    while(1)
    {
        //使用epoll通知内核进行文件ＩＯ检测
        int ret = epoll_wait(epfd,all,sizeof(struct epoll_event),-1); 
        //遍历all数组中的前ret个元素
        for(int i = 0; i < ret ; i++)
        {
            int fd = all[i].data.fd;
        
            //判断是否有新连接
            if(fd == sfd)
            {
                //接受连接请求
                int cfd = accept(sfd,(struct sockaddr*)&client_addr,&client_len);
                if(cfd == -1)
                {
                    perror("accept error!");
                    exit(1);
                }
                //将新得的cfd挂到树上
                struct epoll_event temp;
                temp.events = EPOLLIN;
                temp.data.fd = cfd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&temp);
                
                //打印客户端信息
                char ip[64] = {0};
                printf("New Client Ip: %s, Port: %d\n",\
                       inet_ntop(AF_INET,&client_addr.sin_addr.s_addr ,ip,sizeof(ip)),\
                       ntohs(client_addr.sin_port));
            }
            //已连接的
            else
            {
                //处理已经连接的客户端发过来的数据
                if(!all[i].events & EPOLLIN)
                {
                    continue;
                }
                
                //读数据
                char buf[1024] = {0};
                int len = recv(fd,buf,sizeof(buf),0);
                if(len < 0)
                {
                    perror("recv error");
                    exit(1);
                }
                else if(len == 0)
                {
                    printf("客户端已经断开连接!\n");
                    //fd从树上删掉
                    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
                    if(ret == -1)
                    {
                        perror("epoll_ctl error");
                        exit(1);
                    }
                    close(fd);

                }
                else
                {
                    printf("recv buf: %s\n",buf);
                }
            }

        }
    }



    return 0;
}
