#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <cassert>
#include <sys/epoll.h>
#include <libgen.h>
#include <signal.h>
#include <cstdlib>
#include <cstring>
#include "./threadpool/threadpool.h"

#define MXA_FD 65536             //最大文件描述符
#define MAX_EVENT_NUMBER 10000   //最大事件数

extern int addfd(int epollfd,int fd,bool one_shot);
extern int remove(int epollfd,int fd);
extern int setnoblocking(int fd);

//epoll根节点
static int epollfd = 0;
//父子进程信号管道
static int pipefd[2];

//信号处理函数
void sig_handler(int sig)
{
    //局部变量,保留原来的errno,保证了函数的可重入性
    int save_errno = errno;
    int msg = sig;
    //通过信号管道发送信号
    send(pipefd[1],(char*)&msg,1,0);
    errno = save_errno;
}

void addsig(int sig,void(handler)(int),bool restart = true)
{
    struct sigaction sa;
    memset(&sa,'\0',sizeof(sa));
    sa.sa_handler = handler;
    //信号中断了系统调用，则系统会重新进行系统调用
    if(restart)
        sa.sa_flags |= SA_RESTART;
    //将所有的信号都加入集合当中
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig,&sa,NULL)!=-1);
}

using namespace std;
int main(int argc,char* argv[])
{
    if(argc <= 1)
    {
        printf("usage:%s ip_address port_number\n",basename(argv[0]));
        return 1;
    }
    
    int port = atoi(argv[1]);

    //addsig(SIGPIPE,SIG_IGN);
    
    //创建数据库连接池

    //创建线程池
    threadpool<http_conn> *pool = NULL;
    try
    {
        //connpool数据库连接池
        pool = new threadpool<http_conn>(connpool);
    }
    catch(...)
    {
        return 1;
    }
    
    //保存http请求
    http_conn *users = new http_conn[MAX_FD];
    assert(users);


    //初始化数据库读取表
    

    int listenfd = socket(PF_INET,SOCK_STREAM,0);
    assert(listenfd>=0);

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    int flag = 1;
    //设置套接字属性
    setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    ret = bind(listenfd,(struct sockaddr*)&address,sizeof(address));
    assert(ret >= 0);
    ret = listen(listenfd,5);
    assert(ret >= 0);

    //创建内核事件表
    epoll_event events[MAX_EVENT_NUMBER];
    //创建epoll根节点
    epollfd = epoll_create(5);
    assert(epollfd!=-1);
    
    addfd(epollfd,listenfd,false);
    //static
    http_conn::m_epollfd = epollfd; 

    //创建管道,统一事件源
    ret = socketpair(PF_UNIX,SOCK_STREAM,0,pipefd);
    assert(ret!=-1);
    setnoblocking(pipefd[1]);
    addfd(epollfd,pipefd[0],false);

    //信号集处理
    addsig(SIGALRM,sig_handler,false);
    addsig(SIGTERM,sig_handler,false);
    bool stop_server = false;

    while(!stop_server)
    {
        int number = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
        if(number < 0 && errno != EINTR)
        {
            break;
        }


        for(int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;

            if(sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);

                //ET模式
                
            }
            //出错处理
            else if(events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {

            }

            //信号处理
            else if(events[i].events & EPOLLIN && (sockfd == pipefd[0]))
            {

            }

            //处理客户发来的数据
            else if(events[i].events & EPOLLIN)
            {

            }

            //处理给客户发送的可写事件
            else if(events[i].events & EPOLLOUT)
            {

            }
            else
            {

            }

        }
    }
    close(epollfd);
    close(listenfd);
    close(pipefd[0]);
    close(pipefd[1]);
    delete[] users;
    //释放new出来的线程池
    delete[] pool;

    return 0;
}

