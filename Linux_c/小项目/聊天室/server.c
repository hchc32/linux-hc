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
    ret ++ ;
    sprintf(temp->accounts,"%d",ret);
    printf("接受的数据:%s,%s,%s,%s\n",temp->accounts ,temp->user_name,temp->password ,temp->phone_num);
    
    //数据库操作加锁
    pthread_mutex_lock(&mysql_mutex);
    //然后将数据存放到数据库中
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
        pthread_mutex_unlock(&mysql_mutex);
    }
    mysql_query(&mysql,"use try");
    char temp_mysql[200];
    memset(temp_mysql,0,sizeof(temp_mysql));
    //将帐号和密码存入表中
    sprintf(temp_mysql,"insert into 帐号密码 values('-1','%s','%s','%s','%s','0')"\
            ,temp->accounts,temp->user_name,temp->password,temp->phone_num);
    mysql_query(&mysql,temp_mysql);
    close_mysql(&mysql);               
    if(send((*info)->fd,temp->accounts,sizeof(temp->accounts),0) < 0)
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
    memset(buf,'\0',sizeof(buf));
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
        if(send((*info)->fd,"n\n",sizeof("n\n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else if(strcmp(row[3],serv_log->password) == 0) 
    {
        if(send((*info)->fd,"y\n",sizeof("y\n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
        //填充帐号
       strcpy((*info)->user , serv_log->accounts);
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
        if(send((*info)->fd,"n\n",sizeof("n\n"),0) < 0)
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
            if(send((*info)->fd,"n\n",sizeof("n\n"),0) < 0)
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
            if(send((*info)->fd,"y\n",sizeof("y\n"),0) < 0)
            {
                my_err("send",__LINE__);
            }        
            //将密码发送给客户端
            if(send((*info)->fd,temp_pass,sizeof(temp_pass),0) < 0)
            {
                my_err("send",__LINE__);
            }        
        }
        else
        {
            if(send((*info)->fd,"n\n",sizeof("n\n"),0) < 0)
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
    pthread_exit(NULL);
}


void *add_friend(void *arg)
{
    Sock **info = (Sock**)arg;
    Addfriend *temp = (Addfriend*)(*info)->data;
    char mysql_temp[200];
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
            //f说明friend_accounts不存在
            if(send((*info)->fd,"f\n",sizeof("f\n"),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        else
        {
           // int friend_fd = atoi(row[0]);
            //int flag = atoi(row[5]);
            /*
            Data send_temp;
            memset(&temp,0,sizeof(temp));
            send_temp.type = ADDF;
            strncpy(send_temp.strings,temp->my_accounts,sizeof(temp->my_accounts));
           */
            //在线
           /* if(flag == 1)
            {
                //发送给friend验证消息
                if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
                {
                    my_err("send",__LINE__);
                }
                
            }
            else
            {*/
                //存在消息表中(消息类型:1 -- 好友验证消息 2 -- 消息  3 -- 好友验证回复消息)
                memset(mysql_temp,0,sizeof(mysql_temp));
                sprintf(mysql_temp,"insert into 消息表 values('%s','%s','20','%s')"\
                        ,temp->my_accounts,temp->friend_accounts,temp->my_accounts);
                if(mysql_query(&mysql,mysql_temp) < 0)
                {
                    my_err("mysql_query",__LINE__);
                }
                if(send((*info)->fd,"y\n",sizeof("y\n"),0) < 0)
                {
                    my_err("send",__LINE__);
                }
           // }
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
    //同意加好友
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    if(temp->flag == VALID_USERINFO)
    {
        //更新数据库的好友表
        mysql_query(&mysql,"use try");
        memset(mysql_temp,0,sizeof(mysql_temp));
        sprintf(mysql_temp,"insert into 好友关系 values('%s','%s','0')",temp->my_accounts,temp->friend_accounts);
        if(mysql_query(&mysql,mysql_temp) < 0)
        {
            my_err("mysql_query",__LINE__);
        }
        memset(mysql_temp,0,sizeof(mysql_temp));
        sprintf(mysql_temp,"insert into 好友关系 values('%s','%s','0')",temp->friend_accounts ,temp->my_accounts);
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
        //发送加好友失败信息->my_accounts
        //存在消息表中(消息类型:1 -- 好友验证消息 2 -- 消息  3 -- 好友验证回复消息)
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
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    memset(mysql_temp,0,sizeof(mysql_temp));
    sprintf(mysql_temp,"select *from 好友表 where 帐号=%s",accounts);
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
        }
        if(send((*info)->fd,&count,sizeof(count),0) < 0)
        {
            my_err("send",__LINE__);
        }
        while( (row = mysql_fetch_row(result)) )
        {
            memset(&send_data,0,sizeof(send_data));
            strcpy(temp[i].accounts,row[1]);
            i++;
        }
        for(int j = 0; j < i ; j++)
        {
            memset(mysql_temp,0,sizeof(mysql_temp));
            sprintf(mysql_temp,"select *from 帐号密码 where 帐号=%s",temp[i].accounts);
            MYSQL_RES *result = mysql_store_result(&mysql);
            row = mysql_fetch_row(result);
            strcpy(temp[i].user_name,row[2]);
            temp[i].flag = atoi(row[4]);
        }
        for(int j = 0; j < i; j++)
        {
            memcpy(send_data.strings,&temp[i],sizeof(send_data.strings));
            if(send((*info)->fd,&send_data,sizeof(send_data),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
}

void *data_box(void *arg)
{
    Sock **info = (Sock**)arg;
    char accounts[10];
    char mysql_temp[200];
    s_data temp;
    Data send_temp;
    memset(mysql_temp,0,sizeof(mysql_temp));
    strncpy(accounts,(char*)(*info)->data,sizeof(char)*10);
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
    }
    mysql_query(&mysql,"use try");
    sprintf(mysql_temp,"select *from 消息表 where 帐号2='%s'",accounts);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {
        memset(&temp,0,sizeof(temp));
        memset(&send_temp,0,sizeof(send_temp));
        strcpy(temp.s_accounts,row[0]);
        strcpy(temp.r_accounts,row[1]);
        temp.data_type = atoi(row[2]);
        strcpy(temp.data_string,row[3]);
        memcpy(send_temp.strings,&temp,sizeof(send_temp.strings));
        if(send((*info)->fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    //将accounts的数据发出去
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
}


    
    //改变目前的登录状态
    //树结点的删除

//获取好友名单函数
//在好友列表里寻找，好友１－－好友２　
//　　　　　　　　　好友２－－好友１
//则把她的数据发送过去


//线程回调函数
void* serv_work(void *arg)
{
    Sock *info = (Sock*)arg;
    char ip[10];
    printf("client ip: %s,port: %d\n",
           inet_ntop(AF_INET,&info->addr.sin_addr.s_addr,ip,sizeof(ip)),
           ntohs(info->addr.sin_port));
    while(1)
    {
        //读取客户端发来的数据
        char recv_buf[1024];
        int ret;
        Data recv_data;
        pthread_mutex_lock(&mutex);
        memset(recv_buf,0,sizeof(recv_buf));
        
        ret = recv(info->fd,&recv_buf,sizeof(recv_buf),0);
        if(ret < 0)
        {
            my_err("recv",__LINE__);
        }
        else if(ret == 0)
        {
            continue;
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
            //处理好友请求
           case FRIENDAPPLY:{
                                //解包
                                Addfriend *temp;
                                pthread_t temp_pth;
                                memcpy(&temp,recv_data.strings,sizeof(temp));
                                info->data = &temp;
                                pthread_create(&temp_pth,NULL,friend_apply,(void*)&info);
                                break;
                            }
            //获取好友列表
            case FRIENDLIST:{
                                //解包
                                char *accounts = (char*)malloc(sizeof(char)*10);
                                pthread_t temp_pth;
                                memcpy(&accounts,recv_data.strings,sizeof(accounts));
                                info->data = accounts;
                                pthread_create(&temp_pth,NULL,friend_list,(void*)&info);
                                break;
                            }
                //消息盒子
                case DATABOX:{
                                //解包
                                char *accounts = (char*)malloc(sizeof(char)*10);
                                pthread_t temp_pth;
                                memcpy(&accounts,recv_data.strings,sizeof(accounts));
                                info->data = accounts;
                                pthread_create(&temp_pth,NULL,data_box,(void*)&info);
                                break;
                            }
                //私聊
                
                default:{
                            break;
                        }
            }
        //pthread_mutex_unlock(&mutex);
            //对于接收的数据进行处理
            //好友名单
            //  好友表：　帐号１　　帐号２  　好友关系（１正常好友　０　１把２拉黑）
            //  消息表    帐号1    帐号2     消息类型   消息
            //　好友包
            //｛帐号，昵称　状态｝
            //服务器端获取账号,然后在数据库中寻找对应的好友，再通过帐号表查出昵称和flag，每次将帐号和昵称发过去
            //客户端，循环读区每次发的包，然后存在链表里,再打印出来
            //
            //加好友
            //sfd 帐号 
            //帐号1 + 帐号2
            //1->服务器好友申请
            //2是否在线,是 发送给2好友申请   2拒绝   返给服务器n
            //          否 保存在服务器中    2同意   反给服务器y并且服务器更新好友表
            //
            //好友离开的消息
            //私聊
            //群聊
        }
        pthread_mutex_unlock(&mutex);
    

    return NULL;
}

int main(int argc,char* argv[])
{
    printf("$$$$$4\n");
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
                pthread_create(&id,NULL,serv_work,(void*)&info[i]);
                 
                //进行线程分离
                //pthread_detach(id);
            }
        }
    }
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&mysql_mutex);
    close(sfd);
    //结束主线程
    pthread_exit(NULL);
    return 0;
}

