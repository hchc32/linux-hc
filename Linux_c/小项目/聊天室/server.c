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

//线程回调函数
void* serv_work(void *arg)
{
    Sock *info = (Sock*)arg;
    while(1)
    {
        
        //读取客户端发来的数据
        int buf = -1;
        int len = recv(info->fd,&buf,sizeof(buf),0);
        if(len < 0)
        {
            my_err("recv",__LINE__);
            pthread_exit(NULL);
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
                           regist temp_buf;
                            //再读一次数据,
                            if(recv(info->fd,&temp_buf,sizeof(temp_buf),0) < 0)
                            {
                                my_err("recv",__LINE__);
                                pthread_exit(NULL);
                            }
                            
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
                            char temp[100];
                            //将帐号和密码存入表中
                            sprintf(temp,"insert into 帐号密码 values('%s','%s','%s','%s')",\
                                    temp_buf.accounts,temp_buf.user_name,temp_buf.password,temp_buf.birthday);
                            mysql_query(&mysql,temp);
                            close_mysql(&mysql);
                            //然后向客户端发送帐号
                            char temp_account[10];
                            if(send(info->fd,temp_account,sizeof(temp_account),0) < 0)
                            {
                                my_err("send",__LINE__);
                                pthread_exit(NULL);
                            }
                        
                       }
                //登录功能
                case 2:{

                       }
                //找回密码
                case 3:{

                       }
                //客户端退出
                case 4:{

                       }
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
    int sfd;
    Sock info[2000];
    //获取监听套接字
    sfd = get_lfd();

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    
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
                //接受连接请求
                info[i].fd = accept(sfd,(struct sockaddr*)&cli_addr,&cli_len);
                if(info[i].fd == -1)
                {
                    my_err("accept",__LINE__);
                    exit(1);
                }

                //设置cfd为非阻塞模式
                int flag = fcntl(info[i].fd,F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(info[i].fd,F_SETFL,flag);

                //判断帐号和密码是否正确


                //将新得的fd加入事件表
                struct epoll_event temp;
                //设置边沿触发
                temp.events = EPOLLIN | EPOLLET;
                temp.data.fd = info[i].fd;
                epoll_ctl(epfd,EPOLL_CTL_ADD,info[i].fd,&temp);
                //新成员已经上线
                
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

    close(sfd);
    //结束主线程
    pthread_exit(NULL);
    return 0;
}



