#include"EventLoop.h"
#include<assert.h>
using namespace muduo;

__thread EventLoop *t_loopInThisThread = 0;

EventLoop::EventLoop()
    :looping(false),
      threadId_(CurrentThread::tid())
{
    if(t_loopInThisThread)
    {


    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!looping_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    ::poll(NULL, 0, 5 * 1000);

    looping_ = false;
}