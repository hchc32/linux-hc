#ifndef HTTP_CONN_H
#define HTTP_CONN_H
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include "../lock/lock.h"
class http_conn
{
public:

    //设置读取文件的名称m_real_file的大小
    static const int FILENAME_LEN=200;
    //设置读缓冲区m_read_buf的大小
    static const int READ_BUFFER_SIZE=2048;
    //设置写缓冲区m_write_buf大小
    static const int WRITE_BUFFER_SIZE=1024;
    //报文的请求方法
    enum METHOD
    {
        GET=0,
        POST
    };
    //主状态机的状态(请求行，请求头部，消息体)
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE=0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    //从状态机的状态
    enum LINE_STATUS
    {
        LINE_OK=0,
        LINE_BAD,
        LINE_OPEN
    };
    //报文解析结果
    enum HTTP_CODE
    {
        NO_REQUEST=0, //请求不完整
        GET_REQUEST,  //获得了完整的http请求
        BAD_REQUEST,  //http请求报文有语法错误
        NO_RESOURCE,
        FORBIDDEN_REQUEST,//请求被服务器拒绝
        FILE_REQUEST,
        INTERNAL_ERROR,   //服务器端请求出错
        CLOSED_CONNECTION
    };
public:
    http_conn() {}
    ~http_conn() {}

public:
    //初始化套接字地址
    void init(int sockfd,const sockaddr_in &addr);
    //关闭http连接
    void close_conn(bool read_close=true);
    //工作线程
    void process();

    bool read_once();

    bool write();

    sockaddr_in *get_address()
    {
        return &m_address;
    }


private:
    void init();
    //从m_read_buf读取,并处理请求报文
    HTTP_CODE process_read();
    //向m_write_buf写入响应报文数据
    bool process_write(HTTP_CODE ret);
    //从状态机状态,分析是请求报文的哪一部分
    LINE_STATUS parse_line();

    HTTP_CODE parse_request_line(char *text);
    HTTP_CODE parse_headers(char *text);
    HTTP_CODE parse_content(char *text);
    HTTP_CODE do_request();

    bool add_response(const char * format,...);

    bool add_status_line(int status,const char *title);
    bool add_headers(int content_length);
    bool add_content_type();
    bool add_content_length(int content_length);
    bool add_linger();
    bool add_content(const char *content);
    bool add_blank_line();

    //获取未处理字符
    char *get_line() { return m_read_buf + m_start_line; };

    void unmap();

public:
    static int m_epollfd;
    static int m_user_count;
    MYSQL *mysql;

private:
    //连接socket
    int m_sockfd;
    //客户端地址信息
    sockaddr_in m_address;
    //读缓冲区
    char m_read_buf[READ_BUFFER_SIZE];
    //读缓冲区最后一个数据的下一个位置
    int m_read_idx;
    //m_read_buf中读取的位置
    int m_checked_idx;
    //m_read_buf中已经解析的字符个数
    int m_start_line;
    //存储发出响应报文数据
    char m_write_buf[WRITE_BUFFER_SIZE];
    //提示buffer中的长度
    int m_write_idx;
    //主状态机状态
    CHECK_STATE m_check_state;
    //http请求方法
    METHOD m_method;

    //从请求报文中解析出来的6个数据
    char m_real_file[FILENAME_LEN];
    char *m_url;
    char *m_version;
    char *m_host;
    //用于读取post请求的消息体长度
    int m_content_length;
    //长连接标识符号
    bool m_linger;
    //读取服务器上文件的地址?
    char *m_file_address;
    //文件状态
    struct stat m_file_stat;
    //io向量机制iovec
    struct iovec m_iv[2];
    int m_iv_count;

    int cgi;
    //存储请求头部数据
    char *m_string;
    //剩余发送字节数
    int bytes_to_send;
    //已经发送字节数
    int bytes_have_send;
};
#endif
