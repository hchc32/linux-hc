#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include "./noncopyable.h"
#include <assert.h>
#include <pthread.h>

//from :　https://clang.llvm.org/docs/ThreadSafetyAnalysis.html#basic-concepts-capabilities
#ifndef THREAD_SAFETY_ANALYSIS_MUTEX_H
#define THREAD_SAFETY_ANALYSIS_MUTEX_H

// Enable thread safety attributes only with clang.
// The attributes can be safely erased when compiling with other compilers.
#if defined(__clang__) && (!defined(SWIG))
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   __attribute__((x))
#else
#define THREAD_ANNOTATION_ATTRIBUTE__(x)   // no-op
#endif

#define CAPABILITY(x) \
  THREAD_ANNOTATION_ATTRIBUTE__(capability(x))

#define SCOPED_CAPABILITY \
  THREAD_ANNOTATION_ATTRIBUTE__(scoped_lockable)


//定义检查线程返回值
#ifdef CHECK_PTHREAD_RETURN_VALUE
//__typeof_ == typeof()
#define MCHECK(ret) ({ __typeof_ (ret) errnum = (ret);\
                        if(__builtin_expect(errnum!=0,0))\
                        __assert_perror_fail(errnum,__FILE__,__LINE__,__func__);\
                    
})

#else //CHECK_PTHREAD_RETURN_VALUE

#define MCHECK(ret) ({ __typeof_ (ret) errnum = (ret) \
                        assert(errnum == 0);\ 
                        (void) errnum;\
})

#endif //CHECK_PTHREAD_RETURN_VALUE

namespace muduo
{
class CAPABILITY("mutex") MutexLock : noncopyable
{
public:
    MutexLock()
        :holder_(0)
    {
        MCHECK(pthread_mutex_init(&mutex_, NULL));
    }
    ~MutexLock()
    {
        assert(holder_ == 0);
        MCHECK(pthread_destory(&mutex_));
    }

    //是否是当前线程锁住的
    bool isLockedByThisThread() const
    {
        return holder_ == CurrentThread::tid();
    }

    void assertLocked() const 
    {
        assert(isLockedByThisThread());
    }

    void lock()
    {
        MCHECK(pthread_mutex_lock(&mutex_));
        //赋值holder_
        assignHolder();
    }

    void unlock()
    {
        //holder_=0
        unassignHolder();
        MCHECK(pthread_mutex_unlock(&mutex_));
    }

    pthread_mutex_t * getPthreadMutex()
    {
        return &mutex_;
    }

private:
    friend class Condition;
    
    //管理锁的线程id
    class UnassignGuard : noncopyable
    {
    public:
        explicit UnassignGuard(MutexLock &owner)
            :owner_(owner)
        {
            owner_.unassignHolder();
        }

        ~UnassignGuard()
        {
            owner_.assignHolder();
        }

    private: 
        MutexLock& owner_;
    };

    void unassignHolder()
    {
        holder_ = 0;
    }

    void assignHolder()
    {
        holder_ = CurrentThread::tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;      //锁的持有者
};

//使用互斥锁
class SCOPED_CAPABILITY MutexLockGuard : noncopyable
{
public:
    explicit MutexLockGuard(MutexLock &mutex)
        :mutex_(mutex)
    {
        mutex.lock();
    }

    ~MutexLockGuard()
    {
        mutex.unlock();
    }

private:
    MutexLock &mutex_;
};

}

//防止出现MutexLockGuard(mutex_)
//x为临时对象没有锁住任何东西
#define MutexLockGuard(x) error "Missing guard ogject name"

#endif //MUDUO_BASE_MUTEX_H