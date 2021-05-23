#ifndef MUDUO_BASE_MUTEX_H
#define MUDUO_BASE_MUTEX_H

#include"Thread.h"
#include<pthread.h>
#include<assert.h>
#include<boost/noncopyable.hpp>

namespace muduo
{
    class MutexLock : boost::noncopyable
    {
        public:
            MutexLock()
                :holder_(0)
            {
                pthread_mutex_init(&mutex_, NULL);
            }

            ~MutexLock()
            {
                assert(holder_ == 0);
                pthread_mutex_destroy(&mutex);
            }

            bool isLockedByThisThread()
            {
                return holder_ == CurrentThread::tid();
            }

            void assertLocker()
            {
                assert(isLockedByThisThread());
            }

            void lock()
            {
                pthread_mutex_lock(&mutex_);
                holder_ = CurrentThread::tid();
            }

            void unlock()
            {
                holder_ = 0;
                pthread_mutex_unlock(&mutex_);
            }

            pthread_mutex_t* getPthreadMutex()
            {
                return &mutex_;
            }

        private:
            pthread_mutex_t mutex_; 
            pid_t holder_;

    }

    class MutexLockGuard : boost::noncopyable
    {
        public:
            //防止隐式转换
            explicit MutexLockGuard(MutexLock& mutex) : mutex_(mutex)
            {
                mutex_.lock();
            }

            ~MutexLockGuard()
            {
                mutex_.unlock();
            }

            private:
                MutexLock &mutex_;
                cout << error "ssss" << endl;
    };
}
//静态断言,编译期间的断言
#define MutexLockGuard(x) static_assert(false,"Missint guard object name");

#endif 　//MUDUO_BASE_MUTEX_H