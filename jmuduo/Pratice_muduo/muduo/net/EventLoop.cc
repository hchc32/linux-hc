#include<muduo/net/EventLoop.h>
#include<muduo/base/Logging.h>
#include<muduo/net/Poller.h>
#include<muduo/net/TimerQueue.h>
#include<muduo/base/Timestamp.h>

using namespace muduo;
using namespace muduo::net;

namespace
{
    //当前线程EventLoop对象指针
    //每个线程都有一份
    __thread EventLoop *t_loopInThisThread = 0;

    const int kPollTimeMs = 10000;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

EventLoop::EventLoop()
    : looping_(false),
      quit_(false),
      eventHandling_(false),
      threadId_(CurrentThread::tid()),
      poller_(Poller::newDefaultPoller(this)), //epoll
      currentActiveChannel_(NULL),
      timerQueue_(new TimerQueue(this))
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
    quit_ = false;
    LOG_TRACE << "EventLoop " << this << " start looping";

    //::poll(NULL, 0, 5 * 1000);
    while (!quit_)
    {
        activeChannels_.clear();
        //超时时间10s
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);

        eventHandling_ = true;
        for (Channel*channel:activeChannels_)
        {
            currentActiveChannel_ = channel;
            currentActiveChannel_->handleEvent(pollReturnTime_);
        }
        currentActiveChannel_ = NULL;
        eventHandling_ = false;
    }

    LOG_TRACE << "EventLoop " << this << " stop looping";
    looping_ = false;
}

//该函数可以跨线程调用
void EventLoop::quit()
{
    quit_ = true;
    //不是在io线程调用的,需要唤醒阻塞的io线程
    if(!isInLoopThread())
    {
        //wakeup();
    }
}


//在poller中添加或者更新通道
void EventLoop::updateChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}


//在Poller中移除通道
void EventLoop::removeChannel(Channel *channel)
{
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    if(eventHandling_)
    {

    }
    poller_->removeChannel(channel);
}


void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop" << this
              << "was created in threadId_ = " << threadId_
              << " , current thread id = " << CurrentThread::tid();
}

TimerId EventLoop::runAt(Timestamp time, TimerCallback cb)
{
    return timerQueue_->addTimer(std::move(cb),time,0.0);
}

TimerId EventLoop::runAfter(double delay, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::now(),delay));
    return runAt(time, std::move(cb));
}

TimerId EventLoop::runEvery(double interval, TimerCallback cb)
{
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(std::move(cb), time, interval);
}

void EventLoop::cancel(TimerId timerId)
{
    return timerQueue_->cancel(timerId);
}