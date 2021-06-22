#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include "./base/noncopyable.h"

namespace muduo
{
    class EventLoop : noncopyable
    {
    public:
        EventLoop();
        ~EventLoop();

        //保证某些特定的成员函数只能在特定的线程中使用
        void assertInLoopThread()
        {
            //不在当前线程中
            if(!isInLoopThread())
            {
                //打印错误信息
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() { return threadId_ == CurrentThread::tid(); }

    private:
        void abortNotInLoopThread();
        bool looping_;
        //线程ID
        const pid_t threadId_;
    };
}

#endif  //MUDUO_NET_EVENTLOOP_H