#include <iostream>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
#include "http_conn.h"
using namespace std;


//对文件描述符设置非阻塞
int setnoblocking(int fd)
{
    int old_option = fcntl(fd,F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void addfd(int epollfd,int fd,bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    //EPOLLONESHOT:只监听一次事件，一次处理之后，必须重新挂在树上
    if(one_shot)
        event.events |= EPOLLONESHOT;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
    setnoblocking(fd);
}

void removefd(int epollfd,int fd)
{
    //从树上卸下来
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    //关闭文件描述符号
    close(fd);
}

//重新注册之前的io事件
void modfd(int epollfd,int fd,int ev)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP; //读端关闭事件
    //更改之前ADD的事件
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}
//初始化连接,外部调用初始化套接字地址
void http_conn::init(int sockfd ,const sockaddr_in &addr)
{
    m_sockfd = sockfd;
    m_address = addr;
    addfd(m_epollfd,sockfd,true);
    m_user_count++;
}

//初始化新接受的连接
//check_state默认为分析请求行状态
void http_conn::init()
{
    mysql = NULL;
    //剩余发送字节
    bytes_to_send=0;
    //已发送字节数
    bytes_have_send=0;
    //主状态机状态初始化
    m_check_state=CHECK_STATE_REQUESTLINE;
    //是否长连接
    m_linger=false;
    //请求方法
    m_method=GET;
    m_url=0;
    m_version=0;
    m_content_length=0;
    m_host=0;
    m_start_line=0;
    m_checked_idx=0;
    m_read_idx=0;
    m_write_idx=0;
    cgi=0;
    memset(m_read_buf,'\0',READ_BUFFER_SIZE);
    memset(m_write_buf,'\0',WRITE_BUFFER_SIZE);
    memset(m_real_file,'\0',FILENAME_LEN);
}

void http_conn::close_conn(bool real_close)
{
    if(real_close && (m_sockfd != -1))
    {
        removefd(m_epollfd,m_sockfd);
        m_sockfd = -1;
        m_user_count--;
    }
}

//从状态机状态,用于分析出一行内容
//返回值为行的读取状态,有LINE_OK,LINE_BAD,LINE_OPEN
http_conn::LINE_STATUS http_conn::parse_line()
{
    //m_checked_idx:缓冲区指向当前解析的字符
    //m_read_idx:指向缓冲区最后一个字符的下一个位置
    char temp;
    for(;m_checked_idx < m_read_idx; m_checked_idx++)
    {
        //temp为将要分析的字节
        temp = m_read_buf[m_checked_idx];
        //如果当前是\r字符,则有可能读到完整行
        if(temp == '\r')
        {
            //下一个字符到了buffer结尾,则接收不完整,需要继续接收
            if((m_checked_idx+1)==m_read_idx)
                return LINE_OK;
            //当前行接收完整
            else if(m_read_buf[m_checked_idx+1] == '\n')
            {
                m_read_buf[m_checked_idx++] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }

        //上次接收到的数据为\r,然后到达了缓冲区结尾,本次接收到了\n
        else if(temp == '\n')
        {
            if(m_checked_idx>1 && m_read_buf[m_checked_idx-1] == '\r')
            {
                m_read_buf[m_checked_idx-1] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

//报文解析函数
http_conn::HTTP_CODE http_conn::process_read()
{
   //初始化从状态机的状态和HTTP请求解析报告 
   //从状态机状态
   LINE_STATUS line_status = LINE_OK;
   //HTTP请求解析结果
   HTTP_CODE ret = NO_REQUEST;
   char *text=0;

    //parse_line为从状态机具体实现
    //两种循环条件:适用于GET请求末尾有\r\n
    //             POST请求末尾没有\r\n
    while((m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK) || \
          (line_status = parse_line()) == LINE_OK )
    {
        text = get_line();
        //已经解析的字符=m_read_buf读取的位置
        m_start_line = m_checked_idx;
        //日志书写
        //
        
        //主状态机的三种状态转移逻辑
        switch(m_check_state)
        {
            //解析请求行
            case CHECK_STATE_REQUESTLINE:
            {
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                break;
            }
            //解析请求头部
            case CHECK_STATE_HEADER:
            {
                ret = parse_headers(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                //完整解析GET请求后，跳转到报文响应函数
                else if(ret == GET_REQUEST)
                {
                    return do_request();
                }
                break;
            }
            //解析消息体
            case CHECK_STATE_CONTENT:
            {
                ret = parse_content(text);
                //完整解析post请求后,跳转到报文响应函数
                if(ret == GET_REQUEST)
                {
                    return do_request();
                }
                //解析完消息体即完成报文解析,避免再次进入循环,更新line_status
                line_status = LINE_OPEN;
                break;
            }
            default:
                return INTERNAL_ERRNO;
        }
    }
    return NO_REQUEST;
}

//响应报文生成函数
bool http_conn::process_write()
{

}


void http_conn::process()
{
    //报文解析
    HTTP_CODE read_ret = process_read();

    if(read_ret == NO_REQUEST)
    {
        //注册并监听读事件
        modfd(m_epollfd,m_sockfd,EPOLLIN);
        return;
    }
    //调用process_write完成报文响应
    bool write_ret = process_write(read_ret);
    if(!write_ret)
    {
        close_conn();
    }
    //注册并监听写事件,主线程一直在监听写事件
    modfd(m_epollfd,m_sockfd,EPOLLOUT);
}
