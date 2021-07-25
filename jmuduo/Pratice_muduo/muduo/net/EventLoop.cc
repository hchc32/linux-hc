#include<muduo/net/EventLoop.h>
#include<muduo/base/Logging.h>
#include<poll.h>

using namespace muduo;
using namespace muduo::net;

namespace
{
    //当前线程EventLoop对象指针
    //每个线程都有一份
    __thread EventLoop *t_loopInThisThread = 0;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    : looping_(false),
      threadId_(CurrentThread::tid())
{
    LOG_TRACE << "EventLoop created" << this << "in thread " << threadId_;
    if(t_loopInThisThread)
    {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread
                  << "exists in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    t_loopInThisThread = NULL;
}

//事件循环,该函数不可以跨线程调用
void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    LOG_TRACE << "EventLoop " << this << " start looping";

    ::poll(NULL, 0, 5 * 1000);

    LOG_TRACE << "EventLoop " << this << " stop looping";
    looping_ = false;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop" << this
              << "was created in threadId_ = " << threadId_
              << " , current thread id = " << CurrentThread::tid();
}