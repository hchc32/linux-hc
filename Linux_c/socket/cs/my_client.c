#include<stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include"my_recv.h"

#define INVALID_USERINFO 'n' //用户信息无效
#define VALID_USERINFO   'y' //用户信息有效
/*
 *函数名:get_userinfo
 *描述  :获取用户输入存入到buf中,用户输入的标志以'\n'为结束标志
 *参数  :buf --- 存储用户输入的空间
 *         len --- buf的长度
 *返回值:buf为NULL返回-1,否则返回0
 * */
int get_userinfo(char *buf,int len)
{
    int i = 0;
    int c;
    if(buf == NULL)
    {
        return -1;
    }
    while((c = getchar() != '\n' && (c != EOF) && i < len-2))
    {
        buf[i++] = c;
    }
    buf[i++] = '\n';
    buf[i++] = '\0';

    return 0;
}

/*
 *函数名:input_userinfo
 *描述　:输入用户名，然后通过fd发送出去
 *参数  :conn_fd --- 目标fd
 *         string --- 用户名
 *返回值:无
*/
void input_userinfo(int conn_fd,const char *string)
{
    char input_buf[32];
    char recv_buf[BUFSIZE];
    int flag_userinfo;
    //输入用户信息直到正确为止
    do
    {
        printf("%s:",string);
        if(get_userinfo(input_buf,32) < 0 )
        {
            printf("get_userinfo error! line:%d",__LINE__);
            exit(1);
        }

        if(send(conn_fd,input_buf,strlen(input_buf),0) < 0)
        {
            my_err("send",__LINE__);
        }

        //从连接套接字上读取一次数据
        if(my_recv(conn_fd,recv_buf,sizeof(recv_buf)) < 0)
        {
            printf("my_recv error! line:%d",__LINE__);
            exit(1);
        }
        if(recv_buf[0] == VALID_USERINFO)
        {
            flag_userinfo = VALID_USERINFO;
        }
        else
        {
            printf("%s error,input again",string);
            flag_userinfo = INVALID_USERINFO;
        }
    }while(flag_userinfo == INVALID_USERINFO);
}


int main(int argc,char *argv[])
{
    int i;
    int serv_port;
    int ret;
    int conn_fd;
    char recv_buf[BUFSIZE];
    struct sockaddr_in serv_addr;
    //检查参数个数
    if(argc != 5)
    {
        printf("Usage: [-p] [serv_port] [-a] [serv_address]\n");
        exit(1);
    }

    //初始化服务器端地址结构
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;

    //从命令行获取服务器端的端口与地址
    for(i = 1; i < argc ; i++)
    {
        if(strcmp(argv[i],"-p") == 0)
        {
            serv_port = atoi(argv[i+1]);
            if(serv_port < 0 || serv_port > 65535)
            {
                printf("invalid serv_port.sin_port\n");
                exit(1);
            }
            else
            {
                serv_addr.sin_port = htons(serv_port);
            }
            continue;
        }
        if(strcmp("-a",argv[i]) == 0)
        {
            if(inet_aton(argv[i+1],&serv_addr.sin_addr) == 0)
            {
                printf("invalid serve ip address\n");
                exit(1);
            }
            continue;
        }
    }
    
    /*创建套接字*/
    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd < 0)
    {
        my_err("socker",__LINE__);
    }

    /*向服务器端请求建立连接*/
    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr)) <  0)
    {
        my_err("connect",__LINE__);
    }

    //输入用户名和密码
    input_userinfo(conn_fd,"username");
    input_userinfo(conn_fd,"password");

    /*读取欢迎信息并打印出来*/
    if((ret = my_recv(conn_fd,recv_buf,sizeof(recv_buf))) < 0)
    {
        printf("data is too long\n");
        exit(1);
    }
    for(i = 0; i < ret ; i++)
    {
        printf("%c",recv_buf[i]);
    }
    printf("\n");

    /*关闭连接*/
    close(conn_fd);
    return 0;
}

