#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include"my_error.h"
#include"my_socket.h"

#define SERV_PORT 4507
#define LISTEN_NUM 2000


/*
 *函数名:get_lfd
 *描述  :获得一个监听套接字
 *参数  :无
 *返回值:成功返回一个监听套接字,失败返回-1
*/ 
int get_lfd(void)
{
    int sfd;
    socklen_t serv_len;
    struct sockaddr_in serv_addr;
    //创建一个套接字
    if((sfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
    {
        my_err("socket",__LINE__);
        return -1;
    }
    serv_len = sizeof(serv_addr);
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt = 1;
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    
    //绑定到本地端口
    if(bind(sfd,(struct sockaddr*)&serv_addr,serv_len) < 0)
    {
        my_err("bind",__LINE__);
        return -1;
    }

    //转化为倾听套接字
    if(listen(sfd,LISTEN_NUM) < 0)
    {
        my_err("listen",__LINE__);
        return -1;
    }
    printf("Start accept ......\n");
    return sfd;
}

/*
 *函数名:my_recv
 *描述  :从套接字上读取一次数据
 *参数  :conn_fd -- 从该套接字接受数据
 *             buf -- 存储接受的数据的缓冲区
 *                len -- 缓冲区的大小
 *返回值:成功返回接收到的字数,客户端断开连接返回0,失败返回-1
int my_recv(int conn_fd,void *data_buf,int len)
{
    char buf[BUFSIZE];
    char *pread;
    int recv_len = 0;

    if((recv_len = recv(conn_fd,buf,sizeof(buf),0)) < 0)
    {
        my_err("recv",__LINE__);
        return -1;
    }
    if(recv_len == 0)
    {
        return 0;
    }
    pread = buf;
    for(int i = 0 ; i < recv_len ; i++)
    {
        if(i > len)
        {
            return -1;
        }
        data_buf[i] = *pread++;
    }
    return recv_len;
}
*/
/*
 *函数名:my_send
 *描述  :
 *参数  :conn_fd -- 往该套接字发送数据
 *          data_buf --　发送数据的内容
 *                 len -- 发送数据的大小 
 *返回值:成功返回0,失败返回-1.
int my_send(int conn_fd,void *data_buf,int len)
{
    int i;
    char buf[BUFSIZE];
    char *pread;
    pread = data_buf;
    for( i = 0 ; i < len ; i++)
    {
        buf[i] = *pread++;
    }
    buf[i] = '\0';
    if((send(conn_fd,buf,len,0)) < 0)
    {
        my_err("send",__LINE__);
        return -1;
    }
    return len;

}
*/
