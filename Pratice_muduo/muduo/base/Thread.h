#ifndef MUDUO_BASE_THREAD_H
#define MUDUO_BASE_THREAD_H

#include "./noncopyable.h"
#include <functional>
#include <pthread.h>
#include <string>
#include <atomic>


namespace muduo
{

//线程类
class Thread : noncopyable
{

public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(ThreadFunc, const std::string &name = string());
    ~Thread();

    void start();
    int join();

    static int numCreated() { return numCreated_; }

private:
    void setDefaultName();

    bool start_;            //线程开始标识
    bool joined_;             //线程回收标识
    pthread_t pthreadId_;   //pthread_create()第一个参数
    pid_t tid_;             //线程id
    ThreadFunc func_;       //回调函数
    std::string name_;           //线程名称

    static std::atomic<int> numCreated_; //线程数量
};

} //namespace muduo



#endif