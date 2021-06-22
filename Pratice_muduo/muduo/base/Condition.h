#ifndef MUDUO_BASE_CONDITION_H
#define MUDUO_BASE_CONDITION_H

#include "./Mutex.h"


namespace muduo
{

//条件变量
class Condition : noncopyable
{

public:
    explicit Condition(MutexLock& mutex)
        :mutex_(mutex)
    {
        MCHECK(pthread_cond_init(&pcond_, NULL));
    }

    ~Condition()
    {
        MCHECK(pthread_cond_destroy(&pcond_));
    }

    void wait()
    {
        //提前将该锁的持有者清0
        MutexLock::UnassignGuard ug(mutex_);
        //条件变量阻塞之前会释放锁,被唤醒之后重新持有该锁
        MCHECK(pthread_cond_wait(&pcond_, mutex.getPthreadMutex()));
    }

    void waitForSeconds(double seconds);

    void notify()
    {
        MCHECK(pthread_cond_signal(&pcond_));
    }

    void notifyAll()
    {
        MCHECK(pthread_cond_broadcast(&pcond_));
    }

private:
    MutexLock &mutex_;
    pthread_cond_t pcond_;
};
} // namespace muduo

#endif