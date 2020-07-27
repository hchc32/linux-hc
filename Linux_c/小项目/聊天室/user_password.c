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
int JoinMysql(MYSQL *mysql, Regist *info)
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
    sprintf(temp,"insert into 帐号密码 values('%s','%s','%s','%s')",info->accounts,info->user_name,info->password,info->phone_num);
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
 *函数名:login_userinfo
 *描述　:输入用户信息，然后通过fd发送出去
 *参数  :conn_fd --- 目标fd
 *返回值:信息正确返回１，否则返回0
*/
int login_userinfo(int conn_fd)
{
    Data temp;
    Log input;
    char buf[BUFSIZE];
    memset(&temp,0,sizeof(temp));
    memset(&input,'\0',sizeof(input));
    memset(buf,'\0',sizeof(buf));
    printf("帐号:\n");
    scanf("%s",input.accounts);
    printf("密码:\n");
    scanf("%s",input.password);
    
    temp.type = 2;
    memcpy(temp.strings,&input,sizeof(temp.strings));
    if(send(conn_fd,&temp,sizeof(buf),0) < 0)
    {
        my_err("send",__LINE__);
    }
    //从连接套接字上读取一次数据
    if(recv(conn_fd,buf,sizeof(buf),0) < 0)
    {
        printf("my_recv error! line:%d",__LINE__);
    }
    if(buf[0] == VALID_USERINFO)                       
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 *函数名:find_userinfo
 *描述:给conn_fd发送密保等信息
 *参数：conn_fd -- 目标fd
 *返回值:正确返1,错误返回0
*/ 
int find_userinfo(int conn_fd)
{
    Data temp;
    FindPass input;
    char buf[BUFSIZE];
    memset(&temp,0,sizeof(temp));
    memset(&input,'\0',sizeof(input));
    memset(buf,'\0',sizeof(buf));
    printf("帐号:\n");
    scanf("%s",input.accounts);
    printf("密保答案:\n");
    scanf("%s",input.phone_num);

    temp.type = FINDPASS;
    memcpy(temp.strings,&input,sizeof(temp.strings));
    if(send(conn_fd,&temp,sizeof(buf),0) < 0)
    {
        my_err("send",__LINE__);
    }
    //从连接套接字上读取一次数据
    if(recv(conn_fd,buf,sizeof(buf),0) < 0)
    {
        printf("my_recv error! line:%d",__LINE__);
    }
    if(buf[0] == VALID_USERINFO)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


