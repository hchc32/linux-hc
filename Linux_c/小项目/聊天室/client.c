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
#include"meun.h"
#include"user_password.h"

#define BUFSIZE 1024
#define INVALID_USERINFO 'n' //用户信息无效
#define VALID_USERINFO   'y' //用户信息有效


void *recv_data(void *arg)
{
    while(1)
    {
        if(recv(servfd,))
    }
}



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
    //用fsd和服务器通信
     
    //和服务器通信
    
    //打印初始界面
    int select = 0;
    regist client_regist;
    while(1)
    {
        Log_In_Menu();
        printf("请输入你要操作的功能:");
        scanf("%d",&select);
        switch(select)
        {
            //注册功能
            case 1:{
                        memset(&client_regist,0,sizeof(client_regist));
                        client_regist.cs_flag = 1;
                        printf("请输入你要注册的帐号昵称:\n");
                        scanf("%s",client_regist.user_name);
                        
                        do
                        {
                            printf("请输入你的密码(不小于7位字符):\n");
                            scanf("%s",client_regist.password);
                        }
                        while(strlen(client_regist.password) < 7);

                        printf("密保问题:\n");
                        printf("你的生日是多少?(XXXX-XX-XX)\n:");
                        scanf("%s",client_regist.birthday);
                        
                        //向服务器发送数据
                        if(send(sfd,(void*)&client_regist,sizeof(client_regist),0) < 0)
                        {
                            my_err("send",__LINE__);
                            exit(1);
                        }

                        //接受服务器端的数据
                        int recv_len = 0;
                        char recv_buf[10];
                        
                        if((recv_len = recv(sfd,recv_buf,sizeof(recv_buf),0)) < 0)
                        {
                            my_err("recv",__LINE__);
                            exit(1);
                        }
                        strcpy(client_regist.accounts,recv_buf);
                        printf("你的帐号注册成功,请牢记你的帐号!\n");
                        printf("帐号为:%s\n",client_regist.accounts);
                   }
            //登录功能
            case 2:{
                        input_userinfo(sfd,"帐号");
                        input_userinfo(sfd,"密码");
                   }
            //找回密码
            case 3:{
                        //通过正确的生日和手机号码来找回密码
                        printf("请输入正确的密保答案以找回密码!\n");
                        input_userinfo(sfd,"生日");
                        input_userinfo(sfd,"手机号码");
                   }
            case 4:{
                        printf("系统即将退出!\n");
                        sleep(1);
                        exit(1);
                   }
            default:{
                        printf("请输入正确的选项!\n");
                        getchar();
                        continue;
                    }
        }
    }

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

