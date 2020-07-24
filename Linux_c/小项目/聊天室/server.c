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
    pthread_t id;              //线程id
}Sock;
MYSQL mysql;
pthread_mutex_t mutex;
pthread_mutex_t mysql_mutex;


//注册线程函数
void* registers(void* arg)
{
    Sock **info = (Sock**)arg;
    regist temp_buf;
    //再读一次数据,
    if(recv((*info)->fd,&temp_buf,sizeof(temp_buf),0) < 0)
    {
        my_err("recv",__LINE__);
        pthread_exit(NULL);
    }
    printf("接受的数据:%s,%s,%s\n",temp_buf.user_name ,temp_buf.password,temp_buf.birthday);
    
    //数据库操作加锁
    pthread_mutex_lock(&mysql_mutex);
    //然后将数据存放到数据库中
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
        pthread_exit(NULL);
    }
        mysql_query(&mysql,"use try");
    //获得表中所有数据
    mysql_query(&mysql,"select *from 帐号密码");        
    MYSQL_RES *result = mysql_store_result(&mysql);

    //获取表的列数
    unsigned int num_fields = mysql_num_fields(result);

    //读取每一行数据
    MYSQL_ROW row ;
    while((row = mysql_fetch_row(result)))
    {
        strcpy(temp_buf.accounts,row[0]);
    }
    int change = atoi(temp_buf.accounts);
    change++ ;
    sprintf(temp_buf.accounts,"%d",change);
    char temp[200];
    memset(temp,0,sizeof(temp));
    //将帐号和密码存入表中
    sprintf(temp,"insert into 帐号密码 values('%s','%s','%s','%s')"\
            ,temp_buf.accounts,temp_buf.user_name,temp_buf.password,temp_buf.birthday);
    mysql_query(&mysql,temp);
    close_mysql(&mysql);               

    //数据库操作解锁
    pthread_mutex_unlock(&mysql_mutex);
    //然后向客户端发送帐号                                     
    if(send((*info)->fd,temp_buf.accounts,sizeof(temp_buf.accounts),0) < 0)
    {
        my_err("send",__LINE__);
        pthread_exit(NULL);
    }   
    pthread_exit(NULL);
}

//登录线程函数
void *sign_in(void *arg)
{
    Sock **info = (Sock**)arg;
    
    //接受用户的帐号
    char temp_buf[20];
    memset(temp_buf,'\0',sizeof(temp_buf));
    
    //数据库操作加锁
    pthread_mutex_lock(&mysql_mutex);
    
    //连接数据库
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
        //pthread_exit(NULL);
    }
    
    //接收客户端发的帐号
    if(recv((*info)->fd,temp_buf,sizeof(temp_buf),0) < 0)
    {
        my_err("recv",__LINE__);
       // pthread_exit(NULL);
    }
    printf("帐号%s\n",temp_buf);

    //帐号 昵称 密码 在现状态
    //判断帐号是否存在
    
    mysql_query(&mysql,"use try");
    char mysql_temp[200];
    memset(mysql_temp,'\0',sizeof(mysql_temp));
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号=%s",temp_buf);
   
    mysql_query(&mysql,mysql_temp);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(strcmp(row[0],temp_buf) != 0) 
    {
        if(send((*info)->fd,"n\n",sizeof("n\n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else 
    {
        if(send((*info)->fd,"y\n",sizeof("y\n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
        char temp_buf1[20];
        memset(temp_buf1,'\0',sizeof(temp_buf1));
        if(recv((*info)->fd,temp_buf1,sizeof(temp_buf1),0) < 0)
        {
            my_err("recv",__LINE__);
           // pthread_exit(NULL);
        }
        //判断密码
        if(strcmp(row[2],temp_buf1) != 0)
        {
            if(send((*info)->fd,"n",sizeof("n"),0) < 0)
            {
                my_err("send",__LINE__);
            }
        }
        else
        {
             if(send((*info)->fd,"y",sizeof("y"),0) < 0)
             {
                my_err("send",__LINE__);
             }
             //填充帐号
             strcpy((*info)->user , temp_buf);
            //登录成功时候还要改变数据库中该帐号对应的状态
            memset(mysql_temp,'\0',sizeof(mysql_temp));
            sprintf(mysql_temp,"update 帐号密码 set flag = 1 where 帐号 = '%s'",temp_buf);
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                my_err("mysql_query",__LINE__);
            }
            
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
    pthread_mutex_lock(&mysql_mutex);
    if(connect_mysql(&mysql) < 0)
    {
        my_err("connect_mysql",__LINE__); 
    }
    Sock *info = (Sock*)arg;
    char temp_buf[10];
    memset(temp_buf,'\0',sizeof(temp_buf));
    if(recv(info->fd,temp_buf,sizeof(temp_buf),0) < 0)
    {
        my_err("recv",__LINE__);
    }
    
    mysql_query(&mysql,"use try");
    char mysql_temp[200];
    memset(mysql_temp,'\0',sizeof(mysql_temp));
    sprintf(mysql_temp,"select *from 帐号密码 where 帐号=%s",temp_buf); 

    //如果帐号不存在
    if(mysql_query(&mysql,mysql_temp) < 0)
    {

        if(send(info->fd,"n",sizeof("n"),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }
    else
    {
        if(send(info->fd,"y",sizeof("y"),0) < 0)
        {
            my_err("send",__LINE__);
        }
        else
        {
            char temp_buf1[20];
            memset(temp_buf1,'\0',sizeof(temp_buf1));
            //接受密保答案
            if(recv(info->fd,temp_buf1,sizeof(temp_buf1),0) < 0)
            {
                my_err("recv",__LINE__);
            }
            //判断密保是否正确
            memset(mysql_temp,'\0',sizeof(mysql_temp));
            sprintf(mysql_temp,"select *from 帐号密码 where 帐号=%s and 生日=%s",temp_buf,temp_buf1); 
            if(mysql_query(&mysql,mysql_temp) < 0)
            {
                if(send(info->fd,"n",sizeof("n"),0) < 0)
                {
                    my_err("send",__LINE__);
                }
            }
            else
            {
                if(send(info->fd,"y",sizeof("y"),0) < 0)
                {
                    my_err("send",__LINE__);
                }
                //获取表中的数据
                mysql_query(&mysql,"select *from 帐号密码");
                MYSQL_RES *result = mysql_store_result(&mysql);
                
                //获取表的列数
                unsigned int num_fields = mysql_num_fields(result);

                //读取每一行的数据
                MYSQL_ROW row;
                char temp_password[20];
                memset(temp_password,'\0',sizeof(temp_password));
                while((row = mysql_fetch_row(result)))
                {
                    if(strcmp(temp_buf,row[0]) == 0)
                    {
                        strcpy(temp_password,row[2]);
                    }
                }

                //将密码发送给客户端
                if(send(info->fd,temp_password,sizeof(temp_password),0) < 0)
                {
                    my_err("send",__LINE__);
                }        
            }
        }
    }
    close_mysql(&mysql);
    pthread_mutex_unlock(&mysql_mutex);
    //接受帐号
        //ｉｆ帐号存在，返回ｙ
            //接受密保答案
                //if正确　返回密码
                //if错误  返回ｎ  
        //ｉｆ不存在返回n
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
    pthread_mutex_unlock(&mutex);
    while(1)
    {
        //读取客户端发来的数据
        int buf = -1;
        int len = recv(info->fd,&buf,sizeof(buf),0);
        printf("buf:%d\n",buf);
        if(len < 0)
        {
            my_err("recv",__LINE__);
            continue;
            //pthread_exit(NULL);
        }
        else if(len == 0)
        {
            printf("客户端已经断开连接\n");
            close(info->fd);
            break;
        }
        else
        {
            //根据客户端发来的ｓｅｌｅｃｔ来选择对应的线程函数
            int select = buf;
            switch(select)
            {
                //注册功能
                case 1:{
                            pthread_t temp_pth;
                            pthread_create(&temp_pth,NULL,registers,(void*)&info);
                            pthread_detach(temp_pth);
                            break;
                       }
                //登录功能
                case 2:{
                            pthread_t temp_pth;
                            pthread_create(&temp_pth,NULL,sign_in,(void*)&info);
                            pthread_detach(temp_pth);
                       }
                //找回密码
                case 3:{
                            pthread_t temp_pth;
                            pthread_create(&temp_pth,NULL,find_password,(void*)info);
                            pthread_detach(temp_pth);
                       }
                //客户端退出
                case 4:{
                           pthread_mutex_lock(&mysql_mutex);
                           connect_mysql(&mysql);
                           
                           //修改对应帐号的状态
                           char mysql_buf[200];
                           memset(mysql_buf,'\0',sizeof(mysql_buf));
                           sprintf(mysql_buf,"update 帐号密码 set flag = 0 where 帐号 = %s ",info->user);
                                 
                           //删除ｅｐｏｌｌ树中的事件 

                           close_mysql(&mysql);
                           pthread_mutex_unlock(&mysql_mutex);
                           
                           pthread_exit(NULL);
                       }
                //
                case 5:{

                       }
                case 6:{
                       
                       }
                case 7:{

                       }
            }
            //对于接收的数据进行处理
            //好友名单
            //好友离开的消息
            //私聊
            //群聊
        }
    
    }
    return NULL;
}

int main(int argc,char* argv[])
{
    printf("$$$$$4\n");
    int sfd;
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
                pthread_mutex_lock(&mutex);
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
                temp.events = EPOLLIN | EPOLLET;
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
                pthread_create(&info[i].id,NULL,serv_work,(void*)&info[i]);
                
                //进行线程分离
                pthread_detach(info[i].id);
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



