#define MY_RECV_C
#include<stdio.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<errno.h>
#include"my_recv.h"

/*自定义错误处理函数*/
void my_err(const char* err_string,int line)
{
    fprintf(stderr,"%d",line);
    perror(err_string);
    exit(1);
}

/*
 * 函数名:my_recv
 * 描述  :从套接字上读取一次数据(以'\n'为结束标志)
 * 参数  :conn_fd ---从该套接字上接受数据
 *      　　data_buf --- 读取的数据保存在此缓冲区中
 *              len --- data_buf所指向的空间长度
 *返回值 :出错返回-1,服务器端已关闭连接则返回0,成功返回读取的字节数
*/
int my_recv(int conn_fd,char *data_buf,int len)
{
    static char recv_buf[BUFSIZE]; //自定义缓冲区
    static char *pread;            //指向下一次读取数据的位置     
    static int len_remain = 0;     //自定义缓冲区中的字节数
    int i;
    //自定义缓冲区没有数据,则从套接字读取数据
    if(len_remain <= 0)
    {
        if((len_remain = recv(conn_fd,recv_buf,sizeof(recv_buf),0)) < 0)
        {
            my_err("recv",__LINE__);
        }
        else if(len_remain == 0) //目的计算机的socket连接关闭
        {
            return 0;
        }
        pread = recv_buf; //初始化指针pread
    }

    for( i = 0; *pread!='\0' ; i++)
    {
        if(i > len)
        {
            return -1;
        }
        data_buf[i] = *pread++;
        len_remain--;
    }
  //len_remain--;
    pread++;
    return i;
}


