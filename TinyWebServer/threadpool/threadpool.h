#ifndef THREADPOLL_H
#define THREADPOLL_H

#include<pthread.h>
#include<list>
#include<iostream>
#include<exception>
#include"../lock/lock.h"
using namespace std;

//线程池类:
template<typename T>
class threadpool
{
public:
    threadpool(connection_pool * connPool , int thread_number = 8,int max_request = 10000);
    ~threadpool();
    bool append(T *request);

private:
    static void *worker(void *arg);
    void run();


private:
    int m_thread_number;         //线程池中的线程数量
    int m_max_requests;          //请求队列中允许的最大请求数
    pthread_t *m_threads;        //描述线程池的数组,大小为m_thread_number
    list<T*> m_workqueue;        //请求队列
    locker m_queuelocker;        //保护请求队列的互斥锁
    sem m_queuestat;             //是否有任务需要处理,信号量同步操作
    bool m_stop;                 //是否结束线程
    connection_pool *m_connPool; //连接的数据库

};

template<typename T>
threadpool<T>::threadpool(connection_pool *connPool,int thread_number,int max_requests):m_thread_number(thread_number),\
                    m_max_requests(m_max_requests),m_stop(false),m_threads(NULL),m_connPool(connPool)
{
    if(thread_number <= 0 || max_requests <= 0)
    {
        throw exception();
    }
    m_threads = new pthread_t[m_thread_number];
    if(!m_threads) throw exception();
    for(int i = 0; i < thread_number; i++)
    {
        printf("create the %dth thread\n",i);
        
        if(pthread_create(m_threads+i,NULL,worker,this)!=0)
        {
            delete[] m_threads;
            throw exception();
        }

        //线程分离,不需要pthread_join()回收
        if(pthread_detach(m_threads[i]))
        {
            delete[] m_threads;
            throw exception();
        }
    }
}

//dtor
template<typename T>
threadpool<T>::~threadpool()
{
    delete[] m_threads;
    m_stop = true;
}

//给工作队列里添加任务
template<typename T>
bool threadpool<T>::append(T *request)
{
    m_queuelocker.lock();
    if(m_workqueue.size() > m_max_requests)
    {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    //v操作,同步操作
    m_queuestat.post();
    return true;
}

//
template<typename T>
void *threadpool<T>::worker(void *arg)
{
    threadpool *pool = (threadpool *)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run()
{
    while(!m_stop)
    {
        //p操作
        m_queuestat.wait();
        //队列加锁
        m_queuelocker.lock();
        if(m_workqueue.empty())
        {
            m_queuelocker.unlock();
            continue;
        }
        T* request = m_workqueue.front();
        m_workqueue.pop_front();
        //队列解锁
        m_queuelocker.unlock();
        if(!request)
            continue;

        //连接数据库池
        //RAII机制｀
        connectionRAII mysqlcon(&request->my_sql,m_connPool);

        //执行任务
        //http_conn::process()
        request->process();
    }
}
#endif
