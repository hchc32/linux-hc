#ifndef _THREAD_H_
#define _THREAD_H_
#include<pthread.h>
#include<functional>


class Thread
{
public:
    typedef std::function<void()> ThreadFunc;
    Thread();
    explicit Thread(const ThreadFunc &func);
    ~Thread();
    //线程启动函数
    void Strat();
    //线程回收函数
    void Join();
    void SetAutoDelete(bool autoDelete);

private:
    //线程回调函数
    static void *ThreadRoutine(void *arg);
    //回调函数逻辑函数
    void Run();
    //线程ID
    pthread_t threadId_;
    bool autoDelete_;
    ThreadFunc func_;
};

#endif //_THREAD_H_