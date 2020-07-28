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
char cli_info[10];

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

    //初始化服务器端地址结构
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    
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
    Regist client_regist;
    while(1)
    {
        int select = 0;
        Log_In_Menu();
        printf("请输入你要操作的功能:");
        scanf("%d",&select);
        switch(select)
        {
            //注册功能
            case REGIST:{
                        memset(&client_regist,0,sizeof(client_regist));
                        printf("请输入你要注册的帐号昵称:\n");
                        scanf("%s",client_regist.user_name);
                        do
                        {
                            printf("请输入你的密码(不小于7位字符):\n");
                            scanf("%s",client_regist.password);
                        }
                        while(strlen(client_regist.password) < 7);

                        printf("密保问题:\n");
                        printf("你的电话号码是多少?(11位数)\n:");
                        scanf("%s",client_regist.phone_num);

                        Data send_string;
                        memset(&send_string,0,sizeof(Data));
                        send_string.type = 1;
                        //结构体转成字符串
                        memcpy(send_string.strings,&client_regist,sizeof(send_string.strings));
                        
                        //向服务器发送数据
                        if(send(sfd,&send_string,sizeof(send_string),0) < 0)
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
                        printf("你的帐号注册成功,请牢记你的帐号!\n");
                        printf("帐号为:%s\n",recv_buf);
                        break;
                   }
            //登录功能
            case LOGIN:{
                        getchar();
                        strcpy( cli_info , login_userinfo(sfd));
                        if((char*) cli_info == NULL)
                        {
                            printf("帐号或密码错误!");
                            break;
                        }
                        printf("登录成功!\n");
                        Fun_Menu();
                        //进入登录模块的功能
                        break;
                   }
            //找回密码
            case FINDPASS:{
                           char recv_buf[20];
                           getchar();
                           if(find_userinfo(sfd) == 0)
                           {
                               printf("密保信息错误!");
                               break;
                           }
                           
                           if(recv(sfd,recv_buf,sizeof(recv_buf),0) < 0)
                           {
                               my_err("recv",__LINE__);
                           }
                           printf("你的密码为:%s\n",recv_buf);
                           break;
                          }
            case EXIT:{
                          Data temp;
                          printf("cli.info%s",cli_info);
                          strcpy(temp.strings,cli_info);
                          temp.type = EXIT;
                          if(send(sfd,&temp,sizeof(Data),0) < 0)
                          {
                              my_err("send",__LINE__);
                          }
                          printf("系统即将退出!\n");
                          sleep(1);
                          exit(1);
                          break;
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

