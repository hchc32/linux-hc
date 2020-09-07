#include <stdio.h>       
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>
#include<sys/socket.h>
#include"mysql.h"
#include"my_socket.h"
#include"user_password.h"
#include"my_error.h"

/*
 *函数名:add_friend
 *描述:string帐号向conn_fd发送添加好友的请求
 *参数:conn_fd -- 套接字
 *        string -- 帐号
 *返回值:成功返回1,失败返回0,friend_accounts不存在返回-1
*/
int add_friend(int conn_fd,char *string)
{
    Data send_temp;
    Addfriend temp;
    char recv_buf[10];
    memset(&temp,0,sizeof(temp));
    printf("请输入你要添加的好友帐号:");
    strcpy(temp.my_accounts,string);
    scanf("%s",temp.friend_accounts);
    send_temp.type = ADDF;
    memcpy(&send_temp.strings,&temp,sizeof(send_temp.strings));
    if(send(conn_fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("send",__LINE__);
    }
    if(recv(conn_fd,recv_buf,sizeof(recv_buf), 0) < 0)
    {
        my_err("recv",__LINE__);
    }
    if(recv_buf[0] == VALID_USERINFO)
    {
        return 1;
    }
    else if(recv_buf[0] == 'f')
    {
        return -1;
    }
}
