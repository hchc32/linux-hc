#ifndef _THREAD_H_
#define _THREAD_H_
#include<pthread.h>

//抽象类
class Thread
{
public:
    Thread();
    virtual ~Thread();
    //线程启动函数
    void Strat();
    //线程回收函数
    void Join();
    void SetAutoDelete(bool autoDelete);

private:
    //线程回调函数
    static void *ThreadRoutine(void *arg);
    //回调函数逻辑函数
    virtual void Run() = 0;
    //线程ID
    pthread_t threadId_;
    bool autoDelete_;
};

#endif //_THREAD_H_