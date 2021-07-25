#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include<muduo/base/noncopyable.h>
#include<muduo/base/CurrentThread.h>
#include<muduo/base/Thread.h>

using namespace muduo;

namespace muduo
{
namespace net
{

class EventLoop : noncopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();

    void assertInLoopThread()
    {
        if(!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const
    {
        return threadId_ == CurrentThread::tid();
    }

    static EventLoop *getEventLoopOfCurrentThread();
private:
    void abortNotInLoopThread();

    bool looping_;  
    const pid_t threadId_;
};

}

}





#endif
