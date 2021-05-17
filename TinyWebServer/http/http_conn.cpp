#include <iostream>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
#include "http_conn.h"
using namespace std;

//定义http响应的一些状态信息
const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

//文件地址
const char *doc_root = "/home/hchc/Desktop/TinyWebServer/TinyWebServer-raw_version/root";
//数据库锁(登录+注册)
locker m_lock;

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

//解析http请求行,获得请求方法.目标url,以及http版本号
http_conn::HTTP_CODE http_conn::parse_request_line(char *text)
{
    //获得url字符串
    m_url = strpbrk(text," \t");
    //报文解析错误
    if(!m_url)
    {
        return BAD_REQUEST;
    }
    *m_url++ = '\0';
    //解析请求方法
    char *method = text;
    if(strcasecmp(method,"GET")==0)
    {
        m_method = GET;
    }
    else if(strcasecmp(method,"POST")==0)
    {
        m_method = POST;
        //cgi=1;
    }
    else
    {
        return BAD_REQUEST;
    }

    //解析目标url
    //删除前面的' ' '\t'
    m_url += strspn(m_url," \t");
    m_version = strpbrk(m_url," \t");
    if(!m_version)
    {
        return BAD_REQUEST;
    }
    *m_version++ = '\0';
    
    //解析协议号
    //删除前面的' ' '\t'
    m_version += strspn(m_version," \t");
    //目前仅支持HTTP/1.1
    if(strcasecmp(m_version,"HTTP/1.1")!=0)
    {
        return BAD_REQUEST;
    }
    //增加http://情况
    if(strncasecmp(m_url,"http://",7)==0)
    {
        m_url+=7;
        m_url=strchr(m_url,'/');
    }

    //增加https情况
    if(strncasecmp(m_url,"https://",8)==0)
    {
        m_url+=8;
        m_url=strchr(m_url,'/');
    }
    if(!m_url || m_url[0]!='/')
    {
        return BAD_REQUEST;
    }

    //当url为/时,显示欢迎界面
    if(strlen(m_url)==1)
    {
        strcat(m_url,"judge.html");
    }

    //请求行处理完毕,将主状态机转移处理请求头部
    m_check_state=CHECK_STATE_HEADER;
    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::parse_headers(char *text)
{
    if(text[0]=='\0')
    {
        //判断是GET还是POST
        if(m_content_length!=0)
        {
            m_check_state = CHECK_STATE_CONTENT;
            //请求不完整
            return NO_REQUEST;
        }
        return GET_REQUEST;
    }
    //解析请求头部连接字段
    else if(strncasecmp(text,"Connection:",11)==0)
    {
        text+=11;
        //跳过空格和\t
        text+=strspn(text," \t");
        if(strcasecmp(text,"keep-alive")==0)
        {
            //长连接标志
            m_linger=true;
        }
    }
    //解析请求头部内容长度字段
    else if(strncasecmp(text,"Content-length:",15)==0)
    {
        text+=15;
        text+=strspn(text," \t");
        m_content_length = atol(text);
    }
    //解析请求头部host字段
    else if(strncasecmp(text,"Host:",5)==0)
    {
        text+=5;
        text+=strspn(text," \t");
        m_host = text;
    }
    else
    {
        //日志输出
    }
    return NO_REQUEST;
}

//解析post请求中的消息体
http_conn::HTTP_CODE http_conn::parse_content(char *text)
{
    if(m_read_idx >= (m_content_length+m_checked_idx))
    {
        text[m_content_length]='\0';
        //保存post请求中最后的用户名和密码
        m_string = text;
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

http_conn::HTTP_CODE http_conn::do_request()
{
    //将初始化的m_real_file赋值为网站根目录
    strcpy(m_real_file, doc_root);
    int len = strlen(doc_root);
    //printf("m_url:%s\n", m_url);
    //找到m_url中/的位置
    const char *p = strrchr(m_url, '/');

    //处理cgi,
    if (cgi == 1 && (*(p + 1) == '2' || *(p + 1) == '3'))
    {

        //根据标志判断是登录检测还是注册检测
        char flag = m_url[1];

        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/");
        strcat(m_url_real, m_url + 2);
        strncpy(m_real_file + len, m_url_real, FILENAME_LEN - len - 1);
        free(m_url_real);

        //将用户名和密码提取出来
        //user=123&passwd=123
        char name[100], password[100];
        int i;
        for (i = 5; m_string[i] != '&'; ++i)
            name[i - 5] = m_string[i];
        name[i - 5] = '\0';

        int j = 0;
        for (i = i + 10; m_string[i] != '\0'; ++i, ++j)
            password[j] = m_string[i];
        password[j] = '\0';

        //同步线程登录校验
        if (*(p + 1) == '3')
        {
            //如果是注册，先检测数据库中是否有重名的
            //没有重名的，进行增加数据
            char *sql_insert = (char *)malloc(sizeof(char) * 200);
            strcpy(sql_insert, "INSERT INTO user(username, passwd) VALUES(");
            strcat(sql_insert, "'");
            strcat(sql_insert, name);
            strcat(sql_insert, "', '");
            strcat(sql_insert, password);
            strcat(sql_insert, "')");

            if (users.find(name) == users.end())
            {

                m_lock.lock();
                int res = mysql_query(mysql, sql_insert);
                users.insert(pair<string, string>(name, password));
                m_lock.unlock();

                if (!res)
                    strcpy(m_url, "/log.html");
                else
                    strcpy(m_url, "/registerError.html");
            }
            else
                strcpy(m_url, "/registerError.html");
        }
        //如果是登录，直接判断
        //若浏览器端输入的用户名和密码在表中可以查找到，返回1，否则返回0
        else if (*(p + 1) == '2')
        {
            if (users.find(name) != users.end() && users[name] == password)
                strcpy(m_url, "/welcome.html");
            else
                strcpy(m_url, "/logError.html");
        }
    }

    if (*(p + 1) == '0')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/register.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '1')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/log.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '5')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/picture.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '6')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/video.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else if (*(p + 1) == '7')
    {
        char *m_url_real = (char *)malloc(sizeof(char) * 200);
        strcpy(m_url_real, "/fans.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));

        free(m_url_real);
    }
    else
        strncpy(m_real_file + len, m_url, FILENAME_LEN - len - 1);

    if (stat(m_real_file, &m_file_stat) < 0)
        return NO_RESOURCE;
    if (!(m_file_stat.st_mode & S_IROTH))
        return FORBIDDEN_REQUEST;
    if (S_ISDIR(m_file_stat.st_mode))
        return BAD_REQUEST;
    int fd = open(m_real_file, O_RDONLY);
    m_file_address = (char *)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return FILE_REQUEST;
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
                return INTERNAL_ERROR;
        }
    }
    return NO_REQUEST;
}

//给m_write_buf格式化写入数据
bool http_conn::add_response(const char *format,...)
{
    //如果写入的内容超出m_write_buf则出错
    if(m_write_idx >= WRITE_BUFFER_SIZE)
    {
        return false;
    }

    //定义可变参数列表
    va_list arg_list;

    //将变量arg_list初始化为传入参数
    va_start(arg_list,format);

    int len = vsnprintf(m_write_buf+m_write_idx,WRITE_BUFFER_SIZE-1-m_write_idx,\
                        format,arg_list);
    //如果写入的数据长度超过缓冲区剩余空间,则报错
    if(len >= (WRITE_BUFFER_SIZE-1-m_write_idx))
    {
        va_end(arg_list);
        return false;
    }

    //更新m_write_idx
    m_write_idx += len;
    va_end(arg_list);
    //日志书写????????
    return true;
}


//取消文件映射
void http_conn::unmap()
{
    if(m_file_address)
    {
        munmap(m_file_address,m_file_stat.st_size);
        m_file_address = 0;
    }
}


bool http_conn::read_once()
{
    //读取数据大于缓冲区大小
    if(m_read_idx >= READ_BUFFER_SIZE)
    {
        return false;
    }
    int bytes_read = 0;
    //读取的字节数
    //ET模式
    while(true)
    {
        bytes_read = recv(m_sockfd,m_read_buf+m_read_idx,READ_BUFFER_SIZE-m_read_idx,0);
        if(bytes_read == -1)
        {
            //暂时没有数据的到来||重启后等待数据的到来
            if(errno == EAGAIN || errno == EWOULDBLOCK)
                break;
            return false;
        }
        else if(bytes_read == 0)
        {
            return false;
        }
        //修改m_read_idx的读取字节数
        m_read_idx += bytes_read;
    }
}


bool http_conn::write()
{
    int temp = 0;
    //要发送的数据长度为0
    if(bytes_to_send == 0)
    {
        modfd(m_epollfd,m_sockfd,EPOLLIN);
        init();
        return true;
    }

    while(1)
    {
        //集中写发送
        temp = writev(m_sockfd,m_iv,m_iv_count);
        if(temp < 0)
        {
            if(errno == EAGAIN)
            {
                modfd(m_epollfd,m_sockfd,EPOLLOUT);
                return true;
            }
            //取消文件映射
            unmap();
            return false;
        }
        //已发送字节数
        bytes_have_send += temp;
        //更新还需发送字节数
        bytes_to_send -= temp;
        //第一个iovec头部信息的数据已经发送完类,发送第二个iovec数据
        if(bytes_have_send >= m_iv[0].iov_len)
        {
            m_iv[0].iov_len=0;
            m_iv[1].iov_base = m_file_address + (bytes_have_send - m_write_idx);
            m_iv[1].iov_len = bytes_to_send;
        }
        else //继续发送第一个头部数据
        {
            //起始地址
            m_iv[0].iov_base = m_write_buf + bytes_have_send;
            m_iv[0].iov_len = m_iv[0].iov_len - bytes_have_send;
        }

        //数据全部发送完
        if(bytes_to_send <= 0)
        {
            //取消文件映射
            unmap();
            //重置EPOLLSHOT
            modfd(m_epollfd,m_sockfd,EPOLLIN);
            //长连接
            if(m_linger)
            {
                init();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

//添加状态行
bool http_conn::add_status_line(int status,const char *title)
{
    return add_response("%s %d %s\r\n","HTTP/1.1",status,title);
}
//添加消息报头
bool http_conn::add_headers(int content_len)
{
    
}
//添加Content-Length,表示响应报文的长度
bool http_conn::add_content_length(int content_len)
{
    return add_response("Content-Length:%d\r\n",content_len);;
}

//添加文本类型,这里是html
bool http_conn::add_content_type()
{
    return add_response("Content-Type:%s\r\n","text/html");
}

//添加连接状态,通知浏览器端是保持连接还是关闭
bool http_conn::add_linger()
{
    return add_response("Connection:%s\r\n",(m_linger==true));
}

//添加空行
bool http_conn::add_blank_line()
{
    return add_response("%s","\r\n");
}

//添加文本content
bool http_conn::add_content(const char *content)
{
    return add_response("%s",content); 
}

//响应报文生成函数
bool http_conn::process_write(HTTP_CODE ret)
{
    switch(ret)
    {
        //内部错误
        case INTERNAL_ERROR:
        {
            //状态行
            add_status_line(500,error_500_title);
            //消息报头
            add_headers(strlen(error_500_form));
            if(!add_content(error_500_form))
                return false;
            break;
        }
        //报文错误,404
        case BAD_REQUEST:
        {
            add_status_line(404,error_404_title);
            add_headers(strlen(error_404_form));
            if(!add_content(error_404_form))
                return false;
            break;
        }
        //资源没有访问权限,403
        case FORBIDDEN_REQUEST:
        {
            add_status_line(403,error_403_title);
            add_headers(strlen(error_403_form));
            if(!add_content(error_403_form))
                return false;
            break;
        }
        //文件存在,200
        case FILE_REQUEST:
        {
            //状态行
            add_status_line(200,ok_200_title);
            //请求的资源存在
            if(m_file_stat.st_size != 0)
            {
                //添加头部
                add_headers(m_file_stat.st_size);
                //第一个iovec指针指向响应报文缓冲区
                m_iv[0].iov_base = m_write_buf;
                m_iv[0].iov_len = m_write_idx;

                //第二个iovec指针指向mmap返回的文件指针
                m_iv[1].iov_base = m_file_address;
                m_iv[1].iov_len = m_file_stat.st_size;
                m_iv_count = 2;
                //发送的全部数据为响应报文头部信息和文件大小
                //剩余字节数
                bytes_to_send = m_write_idx + m_file_stat.st_size;
                return true;
            }
            else
            {
                //请求资源为0,则返回空白html文件
                const char *ok_string = "<html><body></body></html>";
                //添加头部信息
                add_headers(strlen(ok_string));
                if(!add_content(ok_string))
                    return false;
            }
            break;
        }
        default:
        {
            return false;
        }
    }
    //除FILE_REQUEST状态外,其余状态只申请一个iovec,指向响应报文缓冲区
    m_iv[0].iov_base = m_write_buf;
    m_iv[0].iov_len = m_write_idx;
    m_iv_count = 1;
    bytes_to_send = m_write_idx;
    return true;
}

//执行任务
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
