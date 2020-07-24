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
 *函数名:get_user_pass
 *描述  :账户注册
 *参数  :对应的数据库
 *返回值:void
*/
int JoinMysql(MYSQL *mysql, regist *info)
{
    char temp[200];
    //查询帐号密码表中最后一个帐号
    //打开数据库
    if(connect_mysql(mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
        return -1;
    }
    mysql_query(mysql,"use try");
    //获得表中所有数据
    mysql_query(mysql,"select *from 帐号密码");        
    MYSQL_RES *result = mysql_store_result(mysql);

    //获取表的列数
    unsigned int num_fields = mysql_num_fields(result);

    //读取每一行数据
    MYSQL_ROW row ;
    while((row = mysql_fetch_row(result)))
    {
        strcpy(info->accounts,row[0]);
    }
    int change = atoi(info->accounts);
    change++ ;
    sprintf(info->accounts,"%d",change);

    //将帐号和密码存入表中
    sprintf(temp,"insert into 帐号密码 values('%s','%s','%s','%s')",info->accounts,info->user_name,info->password,info->birthday);
    mysql_query(mysql,temp);

    //关闭数据库
    close_mysql(mysql);
    return 1;
}

/*
 *函数名:get_userinfo
 *描述  :获取用户输入存入到buf中,用户输入的标志以'\n'为结束标志
 *参数  :buf --- 存储用户输入的空间
 *         len --- buf的长度
 *返回值:buf为NULL返回-1,否则返回0
*/
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
    char input_buf[20];
    char recv_buf[BUFSIZE];
    int flag_userinfo;
    //输入用户信息直到正确为止
    do
    {
        printf("%s:\n",string);
        //fflush(stdout);
        /*
        if(get_userinfo(input_buf,20) < 0 )
        {
            printf("get_userinfo error! line:%d",__LINE__);
            exit(1);
        }
        */
        scanf("%s",input_buf);

        if(send(conn_fd,input_buf,sizeof(input_buf),0) < 0)
        {
            my_err("send",__LINE__);
        }

        //从连接套接字上读取一次数据
        if(recv(conn_fd,recv_buf,sizeof(recv_buf),0) < 0)
        {
            printf("my_recv error! line:%d",__LINE__);
            exit(1);
        }
        printf("%c\n",recv_buf[0]);
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



