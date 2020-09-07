#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/fcntl.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<mysql/mysql.h>
#include<pthread.h>
#include"my_error.h"
#include"my_socket.h"
#include"mysql.h" 
#include"user_password.h"
#include"group.h"

typedef struct Sock
{
    int fd;                    //客户端对应的套接字
    struct sockaddr_in addr;   //用户的ip地址信息
    char user[10];             //用户的帐号
    void *data;
}Sock;
MYSQL mysql;
pthread_mutex_t mutex;
pthread_mutex_t mysql_mutex;
int exit_flag;
int ret = 1;

//注册线程函数
void* registers(void* arg)
{
    Sock **info = (Sock**)arg;
    Regist *temp =(Regist*)(*info)->data;
    Data send_temp;
    int number;
    char buf[6];
    char temp_mysql[200];
    memset(temp_mysql,0,sizeof(temp_mysql));
    memset(&send_temp,0,sizeof(send_temp));
    //数据库操作加锁
    pthread_mutex_lock(&mysql_mutex);
    //然后将数据存放到数据库中
    if(connect_mysql(&mysql) < 0)
    {
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
        my_err("connect_mysql",__LINE__);
        pthread_mutex_unlock(&mysql_mutex);
    }
    mysql_query(&mysql,"use try");
    memset(buf,0,sizeof(buf));
    mysql_query(&mysql,"select *from 分发号码表");
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    number = atoi(row[0]);
    number++;
    sprintf(temp->accounts,"%d",number);
    sprintf(temp_mysql,"update 分发号码表 set 用户号码=%d",number);
    if(mysql_query(&mysql,temp_mysql) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    printf("接受的数据:%s,%s,%s,%s\n",temp->accounts ,temp->user_name,temp->password ,temp->phone_num);
    
    memset(temp_mysql,0,sizeof(temp_mysql));
    //将帐号和密码存入表中
    sprintf(temp_mysql,"insert into 帐号密码 values('-1','%s','%s','%s','%s','0')"\
            ,temp->accounts,temp->user_name,temp->password,temp->phone_num);
    mysql_query(&mysql,temp_mysql);
    close_mysql(&mysql);               
    send_temp.type = REGIST;
    sprintf(send_temp.strings,"注册成功,你的帐号为%s\n",temp->accounts);
    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("send",__LINE__);
    }
    //数据库操作解锁
    pthread_mutex_unlock(&mysql_mutex);
    //然后向客户端发送帐号                                     
    pthread_exit(NULL);
}

//登录线程函数
void *sign_in(void *arg)
{
    Sock **info = (Sock**)arg;
    //接受用户的帐号
    Log *serv_log = (Log*)(*info)->data;
    char buf[1024];
    char mysql_temp[200];
    Data send_temp;
    memset(&send_temp,0,sizeof(send_temp));
    memset(buf,'\0',sizeof(buf));
    send_temp.type = LOGIN;
    //数据库操作加锁
    pthread_mutex_lock(&mysql_mutex);
    //连接数据库
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    printf("要登录的帐号%s\n",serv_log->accounts);
    printf("要登录的帐号密码%s\n",serv_log->password);
    //判断帐号是否存在
    mysql_query(&mysql,"use try");
    memset(mysql_temp,'\0',sizeof(mysql_temp));
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号=%s",serv_log->accounts);
   
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result=mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    //没有查到帐号
    if(row == NULL)
    {
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else if(strcmp(row[3],serv_log->password) == 0) 
    {
        strcpy(serv_log->user_name,row[2]);
        memcpy(&send_temp.strings,serv_log,sizeof(send_temp.strings));
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
        //填充帐号
       strcpy((*info)->user,serv_log->accounts);
       //登录成功时候还要改变数据库中该帐号对应的状态
       memset(mysql_temp,'\0',sizeof(mysql_temp));
       sprintf(mysql_temp,"update 帐号密码 set flag=1 , socket='%d' where 帐号='%s'",(*info)->fd,serv_log->accounts);
       if(mysql_query(&mysql,mysql_temp) < 0)
       {
           my_err("mysql_query",__LINE__);
       }
    }
    else
    {
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    //关闭数据库
    close_mysql(&mysql);
    //数据库操作解锁
    pthread_mutex_unlock(&mysql_mutex);
    
}

//找回密码线程函数
void *find_password(void *arg)
{
    Sock **info = (Sock**)arg;
    FindPass *temp = (FindPass*)(*info)->data;
    Data send_temp;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = FINDPASS;
    printf("找回密码信息:帐号:%s , 密保答案:%s\n",temp->accounts,temp->phone_num);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__); 
    }
    mysql_query(&mysql,"use try");
    char mysql_temp[200];
    memset(mysql_temp,'\0',sizeof(mysql_temp));
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号=%s",temp->accounts); 
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
            my_err("send",__LINE__);
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        if(row == NULL)
        {
            strcpy(send_temp.strings,"no\n");
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }        
            close_mysql(&mysql);
            pthread_mutex_unlock(&mysql_mutex);
            pthread_exit(NULL);
        }
        char temp_pass[20];
        memcpy(temp_pass,row[3],sizeof(temp_pass));
        if(strcmp(row[4],temp->phone_num) == 0)
        {
            strcpy(send_temp.strings,temp_pass);
            //将密码发送给客户端
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }        
        }
        else
        {
            strcpy(send_temp.strings,"no\n");
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }        
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}

void* exit_user(void* arg)
{
    Sock **info = (Sock**)arg;
    char *temp = (char*)(*info)->data; 
    Data send_temp;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = EXIT;
    printf("将要退出的帐号:%s\n",temp);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    char mysql_temp[200];
    memset(mysql_temp,'\0',sizeof(mysql_temp));
    sprintf(mysql_temp,"update 帐号密码 set flag=0 where 帐号='%s'",temp);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    strcpy(send_temp.strings,"yes\n");
    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("send",__LINE__);
    }        
    close((*info)->fd);
    pthread_exit(NULL);
}


void *add_friend(void *arg)
{
    Sock **info = (Sock**)arg;
    Addfriend *temp = (Addfriend*)(*info)->data;
    Data send_temp;
    char mysql_temp[200];
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = ADDFRIEND;
    memset(mysql_temp,0,sizeof(mysql_temp));
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    printf("%s->%s发好友请求\n",temp->my_accounts,temp->friend_accounts);
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp->friend_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        //没有friend_accounts
        if(row == NULL)
        {
            strcpy(send_temp.strings,"no\n");
            //f说明friend_accounts不存在
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        else
        {
            int friend_fd = atoi(row[0]);
            int flag = atoi(row[5]);
            printf("id %d  flag%d\n",friend_fd,flag);
            //在线
            if(flag == 1)
            {
                strcpy(send_temp.strings,"你有一条验证消息!");
                //发送给friend验证消息
                if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                {
                    my_err("send",__LINE__);
                }
                //存在消息表中(消息类型:20 -- 好友验证消息  21 -- 好友验证回复消息 22 -- 消息
                memset(mysql_temp,0,sizeof(mysql_temp));
                sprintf(mysql_temp,"insert into 消息表 values('%s','%s','20','%s')"\
                        ,temp->my_accounts,temp->friend_accounts,temp->my_accounts);
                if(mysql_query(&mysql,mysql_temp) < 0)
                {
                    my_err("mysql_query",__LINE__);
                }
            }
            else
            {
                //存在消息表中(消息类型:20 -- 好友验证消息  21 -- 好友验证回复消息 22 -- 消息)
                memset(mysql_temp,0,sizeof(mysql_temp));
                sprintf(mysql_temp,"insert into 消息表 values('%s','%s','20','%s')"\
                        ,temp->my_accounts,temp->friend_accounts,temp->my_accounts);
                if(mysql_query(&mysql,mysql_temp) < 0)
                {
                    my_err("mysql_query",__LINE__);
                }
            }
        }
    } 
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}

void *del_friend(void *arg)
{
    Sock **info = (Sock**)arg;
    Addfriend *temp = (Addfriend*)(*info)->data;
    char mysql_temp[200];
    Data send_temp;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = DELFRIEND;
    memset(mysql_temp,0,sizeof(mysql_temp));
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    printf("%s删除%s\n",temp->my_accounts,temp->friend_accounts);
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"delete from 好友关系 where 帐号='%s' and 帐号2='%s'",temp->my_accounts,temp->friend_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
        //删除失败
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        //删除成功
        strcpy(send_temp.strings,"yes\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}

void* look_friend(void *arg)
{
    Sock **info = (Sock**)arg;
    char accounts[10];
    char mysql_temp[200];
    Data send_temp;
    Friend_data temp;
    memset(&temp,0,sizeof(temp));
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = LOOKFRIEND;
    strcpy(accounts,(char*)(*info)->data);
    memset(mysql_temp,0,sizeof(mysql_temp));
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    printf("查询%s的信息\n",accounts);
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
        if(send((*info)->fd,"no\n",sizeof("no\n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
         MYSQL_RES *result = mysql_store_result(&mysql);
         MYSQL_ROW row = mysql_fetch_row(result);
         //没有该用户
         if(row == NULL)
         {
             strcpy(send_temp.strings,"no\n");
            if(send((*info)->fd,&send_temp,sizeof(temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
            close_mysql(&mysql);
            pthread_mutex_unlock(&mysql_mutex);
            pthread_exit(NULL);
         }
         strcpy(temp.accounts,row[1]);
         strcpy(temp.user_name,row[2]);
         temp.flag = atoi(row[5]);
         memcpy(&send_temp.strings,&temp,sizeof(send_temp.strings));
         if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
         {
             my_err("send",__LINE__);
         }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}



void *friend_apply(void *arg)
{
    Sock **info = (Sock**)arg;
    Addfriend *temp = (Addfriend*)(*info)->data;
    char mysql_temp[200];
    if(temp->f_g_flag == ADDF)
        printf("加好友:%s -> %s :%c\n",temp->my_accounts,temp->friend_accounts,temp->flag);
    else
        printf("加群:%s->%s :%c\n",temp->my_accounts,temp->friend_accounts,temp->flag);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    if(temp->flag == VALID_USERINFO)
    {
        if(temp->f_g_flag == ADDF)
        {
            //更新数据库的好友表
            memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"insert into 好友关系 values('%s','%s','3')",temp->my_accounts,temp->friend_accounts);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"insert into 好友关系 values('%s','%s','3')",temp->friend_accounts ,temp->my_accounts);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            memset(mysql_temp,0,sizeof(mysql_temp));
            //更新数据库的消息表
            sprintf(mysql_temp,"insert into 消息表 values('%s','%s','21','%s')"\
                    ,temp->my_accounts,temp->friend_accounts,"对方同意了你的请求!");
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
        }
        else
        {
            //更新群成员表
            printf("%s %s\n",temp->my_accounts,temp->friend_accounts);
            memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"insert into 群成员表 values('%s','%s',0)",temp->my_accounts,temp->friend_accounts);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            memset(mysql_temp,0,sizeof(mysql_temp));

            //更新数据库的消息表
            sprintf(mysql_temp,"insert into 消息表 values('%s','%s','21','%s')"\
                    ,temp->my_accounts,temp->friend_accounts,"对方同意了你的请求!");
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }

        }
    }
    else
    {
        //发送加好友失败信息->my_accounts
        //存在消息表中(消息类型:1 -- 好友验证消息 2 -- 消息  3 -- 好友验证回复消息)
        if(temp->f_g_flag == ADDF)
        memset(mysql_temp,0,sizeof(mysql_temp));
        sprintf(mysql_temp,"insert into 消息表 values('%s','%s','21','%s')"\
                ,temp->my_accounts,temp->friend_accounts,"对方拒绝了你的请求!");
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("mysql_query",__LINE__);
        }
    }
    close_mysql(&mysql);
    pthread_mutex_lock(&mysql_mutex);
}

void *friend_list(void *arg)
{
    Sock **info = (Sock**)arg;
    char accounts[10];
    char mysql_temp[200];
    int count = 0 ;
    Data send_data;
    Friend_data temp[50];
    int i = 0;
    strcpy(accounts,(char*)(*info)->data);
    printf("%s请求好友列表!\n",accounts);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    memset(mysql_temp,0,sizeof(mysql_temp));
    sprintf(mysql_temp,"select *from 好友关系 where 帐号='%s'",accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row ;
        while( (row = mysql_fetch_row(result)) )
        {
            count++;
            strcpy(temp[i].accounts,row[1]);
            i++;
        }
        for(int j = 0; j < i ; j++)
        {
            printf("%s\n",temp[j].accounts);
            //memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp[j].accounts);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            result = mysql_store_result(&mysql);
            row = mysql_fetch_row(result);
            strcpy(temp[j].user_name,row[2]);
            temp[j].flag = atoi(row[5]);
        }
        for(int j = 0; j < i; j++)
        {
            memcpy(send_data.strings,&temp[j],sizeof(send_data.strings));
            send_data.type = FRIENDLIST;
            if(send((*info)->fd,&send_data,sizeof(send_data),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        //end
        send_data.type = FRIENDLIST;
        strcpy(send_data.strings,"no\n");
        if(send((*info)->fd,&send_data,sizeof(send_data),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
}

void *data_box(void *arg)
{
    Sock **info = (Sock**)arg;
    char mysql_temp[200];
    s_data temp;
    Data send_temp;
    send_temp.type = DATABOX;
    int count = 0;
    printf("%s准备读取他的消息!\n",(char*)(*info)->data);
    memset(mysql_temp,0,sizeof(mysql_temp));
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 消息表 where 帐号2='%s'",(char*)(*info)->data);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row ;
    if(row == NULL)
    {
        if(send((*info)->fd,"no\n",sizeof("no\n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    sprintf(mysql_temp,"select *from 消息表 where 帐号2='%s'",(char*)(*info)->data);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    result = mysql_store_result(&mysql);
    while( (row = mysql_fetch_row(result)) )
    {
        count++;
    }
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    result = mysql_store_result(&mysql);
    while( (row = mysql_fetch_row(result)) )
    {
        memset(&temp,0,sizeof(temp));
        memset(&send_temp,0,sizeof(send_temp));
        strcpy(temp.s_accounts,row[0]);
        strcpy(temp.r_accounts,row[1]);
        temp.data_type = atoi(row[2]);
        strcpy(temp.data_string,row[3]);
        memcpy(send_temp.strings,&temp,sizeof(send_temp.strings));
        send_temp.type = DATABOX;
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    memset(&send_temp,0,sizeof(send_temp));
    strcpy(send_temp.strings,"end\n");
    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("sned",__LINE__);
    }
    memset(mysql_temp,0,sizeof(mysql_temp));
    sprintf(mysql_temp,"delete from 消息表 where 帐号2='%s'",(char*)(*info)->data);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    //将accounts的数据发出
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
}


void *private_chat(void *arg)
{
    Sock **info = (Sock**)arg;
    s_data *temp = (s_data *)(*info)->data;
    Data send_temp;
    char mysql_temp[200];
    memset(mysql_temp,0,sizeof(mysql_temp));
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = PRIVCHAT;
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    printf("%s->%s发消息\n",temp->s_accounts,temp->r_accounts);
    mysql_query(&mysql,"use try");
    //更新temp->s_accounts处于私聊状态
    sprintf(mysql_temp,"update 帐号密码 set flag=3 where 帐号='%s'",temp->s_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }

    sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp->r_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        //没有r_accounts
        if(row == NULL)
        {
            //f说明r_accounts不存在
            if(send((*info)->fd,"no\n",sizeof("no\n"),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        else
        {
                int friend_fd = atoi(row[0]);
                int flag = atoi(row[5]);
                //在线
                if(flag == 1)
                {
                    strcpy(send_temp.strings,"你有一条消息!");
                    if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                    {
                        my_err("send",__LINE__);
                    }
                }
                //在现且处于私聊状态
                else if(flag == 3)
                {
                    memcpy(send_temp.strings,&temp,sizeof(send_temp.strings));
                    if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                    {
                        my_err("send",__LINE__);
                    }
                    close_mysql(&mysql);
                    pthread_mutex_unlock(&mysql_mutex);
                    pthread_exit(NULL);
                }
                //存在消息表中(消息类型:20 -- 好友验证消息  21 -- 好友验证回复消息 22 -群验证消息 30 -- 消息,31 - 接受文件消息)
                memset(mysql_temp,0,sizeof(mysql_temp));
                sprintf(mysql_temp,"insert into 消息表 values('%s','%s','30','%s')"\
                        ,temp->s_accounts,temp->r_accounts,temp->data_string);
                if(mysql_query(&mysql,mysql_temp) < 0)
                {
                    my_err("mysql_query",__LINE__);
                }
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}


void *group_chat(void *arg)
{
    Sock **info = (Sock**)arg;
    s_data *temp = (s_data *)(*info)->data;
    Data send_temp;
    char mysql_temp[200];
    memset(mysql_temp,0,sizeof(mysql_temp));
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = GROUPCHAT;
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    printf("群:%s->%s发消息\n",temp->s_accounts,temp->r_accounts);
    mysql_query(&mysql,"use try");
    //更新temp->s_accounts处于群聊状态
    sprintf(mysql_temp,"update 帐号密码 set flag=4 where 帐号='%s'",temp->s_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }

    sprintf(mysql_temp,"select *from 群成员表 where 群号='%s'",temp->r_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        //没有r_accounts
        if(row == NULL)
        {
            //f说明r_accounts不存在
            if(send((*info)->fd,"no\n",sizeof("no\n"),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        else
        {
                sprintf(mysql_temp,"select *from 群成员表 where 群号='%s'",temp->r_accounts);
                if(mysql_query(&mysql,mysql_temp) < 0)
                {
                    my_err("send",__LINE__);
                }
                MYSQL_RES *result = mysql_store_result(&mysql);
                MYSQL_ROW row;
                while(row= mysql_fetch_row(result))
                {
                    int friend_fd = atoi(row[0]);
                    int flag = atoi(row[5]);
                    //在线
                    if(flag == 1)
                    {
                        strcpy(send_temp.strings,"你有一条消息!");
                        if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    //在现且处于群聊状态
                    else if(flag == 4)
                    {
                        memcpy(send_temp.strings,&temp,sizeof(send_temp.strings));
                        if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                        {
                            my_err("send",__LINE__);
                        }
                        close_mysql(&mysql);
                        pthread_mutex_unlock(&mysql_mutex);
                        pthread_exit(NULL);
                    }
                    //存在消息表中(消息类型:20 -- 好友验证消息  21 -- 好友验证回复消息 22 -群验证消息 29 --群聊 30 -- 消息,31 - 接受文件消息)
                    memset(mysql_temp,0,sizeof(mysql_temp));
                    sprintf(mysql_temp,"insert into 消息表 values('%s','%s','29','%s')"\
                            ,temp->s_accounts,temp->r_accounts,temp->data_string);
                    if(mysql_query(&mysql,mysql_temp) < 0)
                    {
                        my_err("mysql_query",__LINE__);
                    }
            }
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}


void *creat_group(void *arg)
{
    Sock **info = (Sock**)arg;
    Regist_Group *temp = (Regist_Group *)(*info)->data;
    Data send_temp;
    char mysql_temp[200];
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = CREATGROUP;
    memset(mysql_temp,0,sizeof(mysql_temp));
    printf("%s创建群\n",temp->group_owner_accounts);
    pthread_mutex_lock(&mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    int number;
    char buf[6];
    memset(buf,0,sizeof(buf));
    mysql_query(&mysql,"select *from 分发号码表");
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    number = atoi(row[1]);
    number++;
    sprintf(buf,"%d",number);
    strncpy(temp->group_num,buf,sizeof(temp->group_num));
    temp->group_num[sizeof(temp->group_num)-1] = '\0';
    sprintf(mysql_temp,"update 分发号码表 set 群号码=%d",number);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    memset(mysql_temp,'\0',sizeof(mysql_temp));
    sprintf(mysql_temp,"insert into 群表 values('%s','%s','%s','%s')",\
            temp->group_num,temp->group_name,temp->group_owner_accounts,temp->group_owner_name);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    strcpy(send_temp.strings,buf);
    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("send",__LINE__);
    }
    sprintf(mysql_temp,"insert into 群成员表 values('%s','%s','1')",\
            temp->group_num,temp->group_owner_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    printf("%s创建群成功!\n",temp->group_owner_accounts);
    close_mysql(&mysql);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *destory_group(void *arg)
{
    Sock **info = (Sock**)arg;
    Del_Group *temp = (Del_Group *)(*info)->data;
    Data send_temp;
    char mysql_temp[200];
    char member[100][10];
    int i = 0;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = DELGROUP;
    memset(mysql_temp,0,sizeof(mysql_temp));
    printf("%s删除群%s\n",temp->accounts,temp->group_num);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 群表 where 群号='%s'",temp->group_num);
    if(mysql_query(&mysql,mysql_temp)  < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL)
    {
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else if(strcmp(row[2] , temp->accounts) == 0)
    {
        memset(mysql_temp,0,sizeof(mysql_temp));
        sprintf(mysql_temp,"delete from 群表 where 群号='%s'",temp->group_num);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            strcpy(send_temp.strings,"no\n");
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        else
        {
            strcpy(send_temp.strings,"yes\n");
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
            memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"select *from 群成员表 where 群帐号=%s",temp->group_num);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            else
            {
                MYSQL_RES *result = mysql_store_result(&mysql);
                MYSQL_ROW row;
                while( row = mysql_fetch_row(result) )
                {
                    strcpy(member[i],row[1]);
                }
            }
            for(int j = 0; j < i; j++)
            {
                sprintf(mysql_temp,"insert from 信息表 values('%s','%s','30','%s')",temp->group_num,member[j],"该群被解散\n");
                if(mysql_query(&mysql,mysql_temp) < 0)
                {
                    my_err("mysql_query",__LINE__);
                }
            }
        }
    }
    else
    {
            strcpy(send_temp.strings,"no\n");
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}

void *apply_add_group(void *arg)
{
    Sock **info = (Sock**)arg;
    Add_Group *temp = (Add_Group *)(*info)->data;
    char mysql_temp[200];
    char manage[6][10];
    Data send_temp;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = ADDGROUP;
    memset(mysql_temp,0,sizeof(mysql_temp));
    printf("%s加群%s\n",temp->accounts,temp->group_num);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    
    sprintf(mysql_temp,"select *from 群成员表 where 群号=%s and 成员权限='1' or 成员权限='2'",temp->group_num);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    else
    {   
        //查找该群的群主和管理员帐号
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        if(row == NULL)
        {
           strcpy(send_temp.strings,"no\n");
           if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0 )
           {
               my_err("send",__LINE__);
           }
           close_mysql(&mysql);
           pthread_mutex_unlock(&mysql_mutex);
           pthread_exit(NULL);
        }
        int i = 0;
        memset(mysql_temp,0,sizeof(mysql_temp));
        sprintf(mysql_temp,"select *from 群成员表 where 群号=%s and 成员权限='1' or 成员权限='2'",temp->group_num);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("mysql_query",__LINE__);
        }
        result = mysql_store_result(&mysql);
        while( (row = mysql_fetch_row(result)) )
        {
            strcpy(manage[i++],row[1]);
        }
        //将验证消息发送给群主和管理员
        for(int j = 0 ; j < i ;j++)
        {
            sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",manage[j]);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {   
                my_err("send",__LINE__);
            }
            MYSQL_RES *result = mysql_store_result(&mysql);
            MYSQL_ROW row = mysql_fetch_row(result);
            int friend_fd = atoi(row[0]);
            int flag = atoi(row[5]);
            //在线
            if(flag == 1)
            {
                strcpy(send_temp.strings,"你有一条验证消息!");
                //发送给friend验证消息                                                        
                if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                {
                    my_err("send",__LINE__);
                }
            }
            memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"insert into 消息表 values('%s','%s','22','%s')",\
                    temp->accounts,manage[j],temp->group_num);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
        }
       strcpy(send_temp.strings,"yes\n");
       if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0 )
       {
           my_err("send",__LINE__);
       }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}

void *look_group(void *arg)
{
    Sock **info = (Sock **)arg;
    //群成员帐号
    Regist_Group data[100];
    char *temp = (char *)(*info)->data;
    char mysql_temp[100];
    Data send_temp;
    int i = 0;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = LOOKGROUP;
    memset(mysql_temp,0,sizeof(mysql_temp));
    memset(&data,0,sizeof(data));
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 群成员表 where 群成员帐号='%s'",temp);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL)
    {
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    sprintf(mysql_temp,"select *from 群成员表 where 群成员帐号='%s'",temp);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    result = mysql_store_result(&mysql);
    while( (row = mysql_fetch_row(result)) )
    {
        strcpy(data[i++].group_num,row[0]);
    }
    /*
    if(send((*info)->fd,&i,sizeof(i),0) < 0)
    {
        my_err("send",__LINE__);
    }
    */
    for(int j = 0; j < i ; j++)
    {
        sprintf(mysql_temp,"select *from 群表 where 群号='%s'",data[j].group_num);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("mysql_query",__LINE__);
        }
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        strcpy(data[j].group_name,row[1]);
        strcpy(data[j].group_owner_accounts,row[2]);
        strcpy(data[j].group_owner_name,row[3]);
    }
    for(int j = 0; j < i; j++)
    {
        memset(&send_temp,0,sizeof(send_temp));
        send_temp.type = LOOKGROUP;
        memcpy(&send_temp.strings,&data[j],sizeof(send_temp.strings));
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = LOOKGROUP;
    strcpy(send_temp.strings,"end\n");
    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("send",__LINE__);
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
    
}

void *look_group_member(void *arg)
{
    //群号
    Sock **info = (Sock**)arg;
    char *temp = (char*)(*info)->data;
    char mysql_temp[100];
    Add_Group member[200];
    Data send_temp;
    int i = 0;
    memset(&send_temp,0,sizeof(send_temp));
    memset(mysql_temp,0,sizeof(mysql_temp));
    memset(member,0,sizeof(member));
    send_temp.type = LOOKGMEMBER;
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 群成员表 where 群号='%s'",temp);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL)
    {
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        result = mysql_store_result(&mysql);
        while( row = mysql_fetch_row(result) )
        {
            strcpy(member[i].group_num,row[0]);
            strcpy(member[i].accounts,row[1]);
            strcpy(member[i].user_name,row[2]);
            member[i].group_member_flag = atoi(row[3]);
            i++;
        }
        for(int j = 0; j < i; j++)
        {
            memset(&send_temp,0,sizeof(send_temp));
            send_temp.type = LOOKGMEMBER;
            memcpy(send_temp.strings,&member[i],sizeof(send_temp.strings));
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        memset(&send_temp,0,sizeof(send_temp));
        send_temp.type = LOOKGMEMBER;
        strcpy(send_temp.strings,"end\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);

}

//设置群管理员
void *group_manager(void *arg)
{
    //我的帐号 成员帐号 群号
    Sock **info = (Sock**)arg;
    Manage *temp = (Manage *)(*info)->data;
    Data send_temp;
    char mysql_temp[200];
    memset(mysql_temp,0,sizeof(mysql_temp));
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = GROUPMEMBER;
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 群表 where 群号='%s'",temp->group_num);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_temp",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    //是群主
    if(row[2] == temp->my_accounts)
    {
        memset(mysql_temp,0,sizeof(mysql_temp));
        //设置管理员权限
        sprintf(mysql_temp,"update  群成员表 set 成员权限='2' where 群成员帐号='%s'",temp->member_accounts);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("mysql_temp",__LINE__);
        }
        strcpy(send_temp.strings,"设置成功!\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }    
        //给群成员发送通知消息
        memset(&send_temp,0,sizeof(send_temp));
        memset(mysql_temp,0,sizeof(mysql_temp));
        sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp->member_accounts);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("send",__LINE__);
        }
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        int friend_fd = atoi(row[0]);
        int flag = atoi(row[5]);
        //在线
        if(flag == 1)
        {
            strcpy(send_temp.strings,"你有一条验证消息!");
            //发送给friend验证消息
            if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        //设置管理员权限
        char temp1[50];
        memset(temp1,0,sizeof(temp1));
        sprintf(temp1,"你被设置为%s群的管理员!",temp->group_num);
        sprintf(mysql_temp,"insert into 消息表 values('%s','%s','30','%s')",temp->my_accounts,temp->member_accounts,temp1);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("mysql_temp",__LINE__);
        }
    }
    else
    {
        strcpy(send_temp.strings,"你没有该权限!\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}

//踢人
void *del_group_member(void *arg)
{
    //我的帐号 对面的帐号 群号 
    Sock **info = (Sock**)arg;           
    Manage *temp = (Manage *)(*info)->data;
    char mysql_temp[200];
    Data send_temp;
    memset(&send_temp,0,sizeof(send_temp));
    send_temp.type = DEMEMBER;
    memset(&mysql_temp,0,sizeof(mysql_temp));
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 群成员表 where 群成员帐号='%s' and 群号='%s'",\
            temp->my_accounts,temp->group_num);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = mysql_fetch_row(result);
        if(row == NULL)
        {
            strcpy(send_temp.strings,"no\n");
            if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        //是群主或者管理员
        if(atoi(row[3]) == 1 || atoi(row[3]) == 2)
        {
            memset(&mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"select *from 群成员表 where 群成员帐号='%s' and 群号='%s'",\
                    temp->member_accounts,temp->group_num);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            else
            {
                MYSQL_RES *result = mysql_store_result(&mysql);
                MYSQL_ROW row = mysql_fetch_row(result);
                if(row == NULL)
                {
                    strcpy(send_temp.strings,"no\n");
                    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
                    {
                        my_err("send",__LINE__);
                    }
                }
                else
                {
                    memset(&mysql_temp,0,sizeof(mysql_temp));
                    sprintf(mysql_temp,"delete from 群成员表 where 群成员帐号='%s' and 群号='%s'",\
                            temp->member_accounts,temp->group_num);
                    if(mysql_query(&mysql,mysql_temp) < 0)
                    {
                        my_err("mysql_query",__LINE__);
                    }
                    strcpy(send_temp.strings,"yes\n");
                    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
                    {
                        my_err("send",__LINE__);
                    }
                    
                    //给群成员发消息
                    sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp->member_accounts);
                    if(mysql_query(&mysql,mysql_temp) < 0)
                    {   
                        my_err("send",__LINE__);
                    }
                    MYSQL_RES *result = mysql_store_result(&mysql);
                    MYSQL_ROW row = mysql_fetch_row(result);
                    int friend_fd = atoi(row[0]);
                    int flag = atoi(row[5]);
                    //在线
                    if(flag == 1)
                    {
                        strcpy(send_temp.strings,"你有一条验证消息!");
                        //发送给friend验证消息                                                       
                        if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                        {
                            my_err("send",__LINE__);
                        }
                    }
                    char temp1[50];
                    sprintf(temp1,"你被踢出了群号为%s的群聊!",temp->group_num);
                    memset(&mysql_temp,0,sizeof(mysql_temp));
                    sprintf(mysql_temp,"insert into 消息表 values('%s','%s',30,'%s')",\
                            temp->group_num,temp->member_accounts,temp1);
                    if(mysql_query(&mysql,mysql_temp) < 0)
                    {
                        my_err("mysql_query",__LINE__);
                    }
                }
            }
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}


//文件传输
void *send_file(void* arg)
{
    Sock **info = (Sock**)arg;
    File_Rs *temp = (File_Rs*)(*info)->data;
    char path[50];
    char buffer[1024];
    char mysql_temp[200];
    Data send_temp;
    memset(path,0,sizeof(path));
    sprintf(path,"./%s",temp->filename);
    printf("filename:%s\n",temp->filename);
    FILE *fp = fopen(path,"w");
    if(NULL == fp)
    {
        printf("文件没找到!\n");
    }
    int len = 0;
    
    pthread_mutex_lock(&mutex);
    bzero(buffer,1024);
    while((len = recv((*info)->fd,buffer,1024,0)) > 0)
    {
        printf("len = %d\n",len);
        if(strcmp(buffer,"end\n") == 0)
        {
            break;
        }
        if(fwrite(buffer,sizeof(char),len,fp) < len)
        {
            printf("文件接受失败!\n");
            break;
        }
        bzero(buffer,1024);
        usleep(1);
    }
    pthread_mutex_unlock(&mutex);
    fclose(fp);
    printf("%srecv access!\n",temp->filename);
    strcpy(send_temp.strings,"yes\n");
    send_temp.type = SENDFILE;
    //发送给friend验证消息
    if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
    {
        my_err("send",__LINE__);
    }
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp->r_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("send",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    int friend_fd = atoi(row[0]);
    int flag = atoi(row[5]);
    //在线
    if(flag == 1)
    {
        strcpy(send_temp.strings,"你有一个待接收的文件");
        //发送给friend验证消息
        if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    memset(mysql_temp,0,sizeof(mysql_temp));
    sprintf(mysql_temp,"insert into 消息表 values('%s','%s','31','%s')",\
            temp->s_accounts,temp->r_accounts,temp->filename);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {
        my_err("mysql_query",__LINE__);
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    pthread_exit(NULL);
}


void *recv_file(void *arg)
{
    Sock **info = (Sock**)arg;
    File_Rs *temp = (File_Rs *)(*info)->data;
    Data send_temp;
    char buff[1024];
    send_temp.type = RECVFILE;
    printf("%s接受%s文件!\n",temp->s_accounts,temp->filename);
    char path[50];
    printf("filename%s\n",temp->filename);
    sprintf(path,"./%s",temp->filename);
    FILE *fp = fopen(path,"w");
    if(fp == NULL)
    {
        printf("open filr error!\n");
        strcpy(send_temp.strings,"no\n");
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        strcpy(send_temp.strings,temp->filename);
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
        bzero(buff,1024);
        int len = 0 ;
        while((len = fread(buff,sizeof(char),1024,fp)) > 0)
        {
            printf("len = %d\n",len);
            if(send((*info)->fd,buff,len,0) < 0)
            {
                my_err("send",__LINE__);
            }
            bzero(buff,1024);
            usleep(1);
        }
        usleep(1);
        strcpy(buff,"end\n");
        if(send((*info)->fd,buff,sizeof(buff),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    printf("文件发送成功!\n");
    fclose(fp);
}


//线程回调函数
void* serv_work(void *arg)
{
    Sock *info = (Sock*)arg;
    char ip[10];
    printf("client ip: %s,port: %d\n",
           inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip)),
           ntohs(info->addr.sin_port));
        //读取客户端发来的数据
        char recv_buf[1050];
        int ret;
        Data recv_data;
        memset(recv_buf,0,sizeof(recv_buf));
        {
        {
            
            ret = recv(info->fd,&recv_buf,sizeof(recv_buf),0);
         //pthread_mutex_unlock(&mutex);
            if(ret < 0)
            {   
                if(errno == 11)
                {
              //      continue;
                }
                else
                {
            //        break;
                }
                //my_err("recv",__LINE__);
            }
        }
        memset(&recv_data,0,sizeof(recv_data));
        memcpy(&recv_data,recv_buf,sizeof(recv_data));
        //根据客户端发来的ｓｅｌｅｃｔ来选择对应的线程函数
        int select = recv_data.type;
        printf("type:%d\n",recv_data.type);
        switch(select)
        {
            //注册功能
            case REGIST:{
                        //解包
                        Regist temp; 
                        pthread_t temp_pth;
                        memcpy(&temp,recv_data.strings,sizeof(temp));
                        info->data = &temp;
                        pthread_create(&temp_pth,NULL,registers,(void*)&info);
                        //pthread_detach(temp_pth);
                        break;
                        }
            //登录功能
            case LOGIN:{
                        //解包
                        Log temp;
                        pthread_t temp_pth;
                        memcpy(&temp,recv_data.strings,sizeof(temp));
                        info->data = &temp;
                        pthread_create(&temp_pth,NULL,sign_in,(void*)&info);
                        //pthread_detach(temp_pth);
                        break;
                        }
            //找回密码
            case FINDPASS:{
                            //解包
                            FindPass temp;
                            pthread_t temp_pth;
                            memcpy(&temp,recv_data.strings,sizeof(temp));
                            info->data = &temp;
                            pthread_create(&temp_pth,NULL,find_password,(void*)&info);
                            // pthread_detach(temp_pth);
                            break;
                          }
            //客户端退出
            case EXIT:{
                        //解包
                        char temp_account[10];
                        pthread_t temp_pth;
                        memcpy(temp_account,recv_data.strings,sizeof(temp_account));
                        info->data = temp_account;
                        pthread_create(&temp_pth,NULL,exit_user,(void*)&info);
                        break;
                   }
            //加好友
            case ADDFRIEND:{
                                //解包
                                Addfriend temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,add_friend,(void*)&info);
                                break;
                            }
            //删除好友
            case DELFRIEND:{
                                //解包
                                //传帐号
                                Addfriend temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,del_friend,(void*)&info);
                                break;

                           }
            //查找好友
            case LOOKFRIEND:{
                                //解包
                                Addfriend temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,look_friend,(void*)&info);
                                break;
                            }
            //处理好友请求
            case FRIENDAPPLY:{
                                //解包
                                Addfriend temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,friend_apply,(void*)&info);
                                break;
                            }
            //获取好友列表
            case FRIENDLIST:{
                                //解包
                                char accounts[10]; 
                                pthread_t temp_pth;
                                memcpy(accounts,recv_data.strings,sizeof(accounts));
                                info->data = accounts;
                                pthread_create(&temp_pth,NULL,friend_list,(void*)&info);
                                break;
                            }
                //消息盒子
                case DATABOX:{
                                //解包
                                pthread_t temp_pth;
                                //char *accounts = (char*)malloc(sizeof(char)*10);
                                char accounts[10];
                                memcpy(accounts,recv_data.strings,sizeof(accounts));
                                info->data = accounts;
                                pthread_create(&temp_pth,NULL,data_box,(void*)&info);
                                //free(accounts);
                                break;
                             }
                //私聊
                case PRIVCHAT:{
                                //解包
                                s_data temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,private_chat,(void*)&info);
                                break;
                              }  
                case GROUPCHAT:{
                                //解包
                                s_data temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,group_chat,(void*)&info);
                                break;
                               }
                //创建群
                case CREATGROUP:
                             {
                                //群表
                                //群号　群昵称 群主帐号 群主昵称
                                //群号　群成员帐号　群成员昵称　　成员权限 　
                                //群消息表　群号　群成员　群消息类型
                                Regist_Group temp;
                                memset(&temp,0,sizeof(temp));
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,creat_group,(void*)&info);
                                break;
                             }
                //删除群
                case DELGROUP:{
                                  Del_Group temp;
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(&temp,recv_data.strings,sizeof(temp));
                                  info->data = &temp;
                                  pthread_create(&temp_pth,NULL,destory_group,(void*)&info);
                                  break;
                              }
                //加群
                case ADDGROUP:{
                                  Add_Group temp;
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(&temp,recv_data.strings,sizeof(temp));
                                  info->data = &temp;
                                  pthread_create(&temp_pth,NULL,apply_add_group,(void*)&info);
                                  break;
                              }
                //查看群
                case LOOKGROUP:{
                                  char temp[6];
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(temp,recv_data.strings,sizeof(temp));
                                  info->data = temp;
                                  pthread_create(&temp_pth,NULL,look_group,(void*)&info);
                                  break;

                               }
                //查看群成员
                case LOOKGMEMBER:{
                                  char temp[6];
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(temp,recv_data.strings,sizeof(temp));
                                  info->data = temp;
                                  pthread_create(&temp_pth,NULL,look_group_member,(void*)&info);
                                  break;
                                 }
                //设置管理员
                case GROUPMEMBER:{
                                  Manage temp;
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(&temp,recv_data.strings,sizeof(temp));
                                  info->data = &temp;
                                  pthread_create(&temp_pth,NULL,group_manager,(void*)&info);
                                  break;
                                 } 
                //踢人
                case DEMEMBER:{
                                  Manage temp;
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(&temp,recv_data.strings,sizeof(temp));
                                  info->data = &temp;
                                  pthread_create(&temp_pth,NULL,del_group_member,(void*)&info);
                                  break;
                              }
                //传文件
                case SENDFILE:{
                                  File_Rs temp;
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(&temp,recv_data.strings,sizeof(temp));
                                  info->data = &temp;
                                  pthread_create(&temp_pth,NULL,send_file,(void*)&info);
                                  pthread_detach(temp_pth);
                                  break;

                              }
                case RECVFILE:{
                                  File_Rs temp;
                                  memset(&temp,0,sizeof(temp));
                                  pthread_t temp_pth;
                                  memcpy(&temp,recv_data.strings,sizeof(temp));
                                  info->data = &temp;
                                  printf("pthreadc\n");
                                  pthread_create(&temp_pth,NULL,recv_file,(void*)&info);
                                  break;
                              }
                default:{
                            break;
                        }
            }
        }
        pthread_exit(NULL);
    

}

int main(int argc,char* argv[])
{
    printf("$$$$$4\n");
    //int ffd = open("data",O_WRONLY|O_CREAT,0666);
    //dup2(ffd,1);
    //fflush(stdout);

    int sfd;
    pthread_t id;
    Sock info[2000];
    //获取监听套接字
    sfd = get_lfd();
    
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&mysql_mutex,NULL);

    //初始化epoll树
    int epfd = epoll_create(2000);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = sfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sfd,&ev);
    
    struct epoll_event events[2000];

    while(1)
    {
        //内核检测事件
        int ret = epoll_wait(epfd,events,sizeof(events),-1);
        //遍历events的前ret个元素
        for(int i = 0; i < ret ; i++)
        {
            int fd = events[i].data.fd;
            //是否有待连接的fd
            if(fd == sfd)
            {
                //pthread_mutex_lock(&mutex);
                //接受连接请求
                info[i].fd = accept(sfd,(struct sockaddr*)&cli_addr,&cli_len);
                if(info[i].fd == -1)
                {
                    my_err("accept",__LINE__);
                    exit(1);
                }
                /*
                //设置cfd为非阻塞模式
                int flag = fcntl(info[i].fd,F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(info[i].fd,F_SETFL,flag);
                */
                //将新得的fd加入事件表
                struct epoll_event temp;
                //设置边沿触发
                temp.events = EPOLLIN | EPOLLET ;
                temp.data.fd = info[i].fd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,info[i].fd,&temp);
            }
            else
            {
                if(!events[i].events & EPOLLIN)
                {
                    continue;
                }
                //创建子线程
                pthread_mutex_lock(&mutex);
                pthread_create(&id,NULL,serv_work,(void*)&info[i]);
                pthread_mutex_unlock(&mutex);
                //进行线程分离
                //pthread_detach(id);
            }
        }
    }
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mysql_mutex);
    close(sfd);
    //结束主线程
    //close(ffd);
    pthread_exit(NULL);
    return 0;
}

