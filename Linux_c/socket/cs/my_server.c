#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include"my_recv.h"

#define SERV_PROT 4507 /*服务器的端口*/
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
void send_data(int conn_de,const char *string)
{
    if(send(conn_de,string,strlen(string),0) < 0)
    {
        my_err("send",__LINE__);
    }
}




int main()
{
    
    return 0;
}

