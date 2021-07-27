#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include<muduo/base/noncopyable.h>
#include<muduo/base/CurrentThread.h>
#include<muduo/base/Thread.h>
#include<muduo/base/Timestamp.h>
#include<muduo/net/Channel.h>
#include<muduo/net/TimerId.h>
#include<vector>
using namespace muduo;
using namespace muduo::net;
namespace muduo
{
namespace net
{


typedef std::function<void()> TimerCallback;
class Channel;
class Poller;
class TimerQueue;

class EventLoop : noncopyable
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    //timers

    //Runs callback at 'time'
    //线程安全的
    TimerId runAt(Timestamp time, TimerCallback cb);

    TimerId runAfter(double delay, TimerCallback cb);

    TimerId runEvery(double interval, TimerCallback cb);
    
    ///
    /// Cancels the timer.
    /// Safe to call from other threads.
    ///
    void cancel(TimerId timerId);





    Timestamp pollReturnTime() const { return pollReturnTime_; }

    void updateChannel(Channel *channel);//在poller中添加或者更新通道
    void removeChannel(Channel *channel); //在Poller中移除通道
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

    void printActiveChannels();

    typedef std::vector<Channel *> ChannelList;

    bool looping_;  //是否处于循环状态
    bool quit_;     //是否退出loop
    bool eventHandling_; //当前是否处于事件处理状态
    const pid_t threadId_; //当前对象所属线程id
    Timestamp pollReturnTime_; //调用poll返回的时间戳
    std::unique_ptr<Poller> poller_; //Poller对象(智能指针)
    ChannelList activeChannels_; //Poller返回的活动通道
    Channel *currentActiveChannel_; //当前正在处理的活动通道

    std::unique_ptr<TimerQueue> timerQueue_;
};
} // namespace net

} // namespace muduo

#endif
