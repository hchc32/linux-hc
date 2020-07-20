#include<stdio.h>     
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<mysql/mysql.h>
#include"my_error.h"
#include"my_socket.h"


int main(int argc,char *argv[])
{
    int sfd;
    int serv_prot;
    struct sockaddr_in serv_addr;
    //检查参数个数
    if(argc < 5)
    {
        printf("./a.out -p serv_prot -a serv_address\n");
        exit(1);
    }
    
    for(int i = 0; i < argc ; i++)
    {
        //命令行获取端口
        if(strcmp(argv[i],"-p") == 0)
        {
            serv_prot = atoi(argv[i+1]);
            if(serv_prot < 0 || serv_prot > 65535)
            {
                printf("无效的prot!\n");
                exit(1);
            }
            else
            {
                serv_addr.sin_port = htons(serv_prot);
            }
        }

        //命令行获取ip地址
        if(strcmp(argv[i],"-a") == 0)
        {
            if(inet_aton(argv[i+1],&serv_addr.sin_addr) == 0)
            {
                printf("无效的ip地址!\n");
                exit(1);
            }
            continue;
        }
    }

    //创建套接字
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd < 0)
    {
        my_err("socket",__LINE__);
        exit(1);
    }
    
    //向服务器端请求建立连接
    if(connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        my_err("connect",__LINE__);
        exit(1);
    }

    //和服务器通信
    //打印初始界面
    //1.注册
    //2.登录
    //3.找回密码
    //4.退出
    //
    //进入主界面
    //1.获取好友名单
    //2.私聊
    //群聊
    //加好友
    //删除好友
    //删除群成员
    //加群
    //发文件
    
    return 0;
}

