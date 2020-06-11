#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include"my_recv.h"
#include<linux/socket.h>

#define SERV_PORT 4507 /*服务器的端口*/
#define LISTENQ   12   /*连接请求队列的最大长度*/

/*保存用户名和密码的结构体*/
struct usersinfo
{
    char username[32];
    char password[32];
};

/*已存在的用户名和密码*/
struct usersinfo users[] = 
{
    {"linux","unix"},
    {" "," "}
};

/*
 * 函数名:find_name
 * 描述  :查找用户名是否存在
 * 参数  :name --- 用户名
 * 返回值:name为NULL时返回-2,用户名不存在返回-1,否则返回用户名下标
*/
int find_name(const char* name)
{
    int i;
    if(name == NULL)
    {
        return -2;
    }
    for(i = 0; users[i].username[0] != ' ' ; i++)
    {
        if(strcmp(name,users[i].username) == 0)
        {
            return i;
        }
    }
    return -1;
}

/*
 *函数名:send_data
 *描述  :给conn_fd套接字发送数据
 *参数  :conn_fd --- 客户端套接字
 *         string --- 数据内容
 *返回值:void
*/
void send_data(int conn_fd,const char *string)
{
    if(send(conn_fd,string,strlen(string),0) < 0)
    {
        my_err("send",__LINE__);
    }
}



int main()
{
    int socker_fd;                 
    int option_value;
    int conn_fd;
    struct sockaddr_in client_addr,server_addr;
    socklen_t cli_len;
    
    /*创建一个套接字*/ 
    if((socker_fd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        my_err("socket",__LINE__);
    }

    /*设置该套接字,使之可以重新绑定端口*/
    option_value = 1;
    if((setsockopt(socker_fd,SOL_SOCKET,SO_REUSEADDR,\
       (const void*)&option_value,sizeof(option_value))) < 0)
    {
        my_err("setsockopt",__LINE__);
    }

    /*初始化服务器端地址结构*/
    memset(&server_addr,0,sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /*绑定一个套接字到本地端口*/
    if(bind(socker_fd,(struct sockaddr *)&server_addr,sizeof(server_addr)) < 0)
    {
        my_err("bind",__LINE__);
    }

    /*套接字转化为被动监听*/
    if( listen(socker_fd,LISTENQ) < 0)
    {
        my_err("listen",__LINE__);
    }

    /*阻塞等待客户端的连接请求*/
    cli_len = sizeof(struct sockaddr_in);
    while(1)
    {
        //通过accept接受客户端的连接请求，并返回连接套接字用于收发数据
        conn_fd = accept(socker_fd,(struct sockaddr*)&client_addr,&cli_len);
        if(conn_fd < 0)
        {
            my_err("accept",__LINE__);
        }
        printf("accept a new client, ip:%s\n",inet_ntoa(client_addr.sin_addr));

        
    }


    /*读取客户端数据*/

    /*向客户端写数据*/

    /*关闭连接*/
    return 0;
}

