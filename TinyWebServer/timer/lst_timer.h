#ifndef LST_TIMER
#define LST_TIMER
#include<time.h>
#include"../log/log.h"

//声明定时器类
//class util_timer;

struct client_data
{
    //客户端地址
    sockaddr_in address;
    //socket文件描述符号
    int sockfd;
    //定时器
    util_timer *timer;
};



class util_timer
{
public:
    util_timer() {}
    ~util_timer() {}

    //超时时间
    time_t expire;
    //回调函数
    void (*cb_func)(client_data*);
    //连接资源
    client_data *user_data;
    //前向定时器
    util_timer *prev;
    //后继定时器
    util_timer *next;
};

//带头尾节点的升序双向链表
//定时器容器
class sort_timer_lst
{
public:
    sort_timer_lst():head(NULL),tail(NULL) {  }

    ~sort_timer_lst()
    {
        util_timer *tmp = head;
        while(tmp)
        {
            head = tmp->next;
            delete tmp;
            tmp = head;
        }
    }


private:
    //被共有成员调用，主要用于调整链表内部节点
    void add_timer(util_timer *timer,util_timer *lst_head)
    {
        util_timer *prev = lst_head;
        util_timer *tmp = prev->next;
        while(tmp)
        {
            if(timer->expire < tmp->expire)
            {
                prev->next = timer;
                timer->next = tmp;
                tmp->prev = timer;
                timer->prev = prev;
                break;
            }
            prev = tmp;
            tmp = tmp->next;
        }
        if(!tmp)
        {
            prev->next = timer;
            timer->prev = prev;
            timer->next = NULL;
            tail = timer;
        }
    }
private:
    //双向链表头尾节点
    util_timer *head;
    util_timer *tail;
};

#endif
