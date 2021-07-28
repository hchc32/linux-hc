// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif

#include "muduo/net/TimerQueue.h"

#include "muduo/base/Logging.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/Timer.h"
#include "muduo/net/TimerId.h"

#include <sys/timerfd.h>
#include <unistd.h>

namespace muduo
{
namespace net
{
namespace detail
{

int createTimerfd()
{
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }
  return timerfd;
}

//延迟100ms
struct timespec howMuchTimeFromNow(Timestamp when)
{
  int64_t microseconds = when.microSecondsSinceEpoch()
                         - Timestamp::now().microSecondsSinceEpoch();
  if (microseconds < 100)
  {
    microseconds = 100;
  }
  struct timespec ts;
  ts.tv_sec = static_cast<time_t>(
      microseconds / Timestamp::kMicroSecondsPerSecond);
  ts.tv_nsec = static_cast<long>(
      (microseconds % Timestamp::kMicroSecondsPerSecond) * 1000);
  return ts;
}

//读取超时时间
void readTimerfd(int timerfd, Timestamp now)
{
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
  }
}

//重新设置系统定时器时间
void resetTimerfd(int timerfd, Timestamp expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  memZero(&newValue, sizeof newValue);
  memZero(&oldValue, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration); //实际超时的时间早.
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
    LOG_SYSERR << "timerfd_settime()";
  }
}

}  // namespace detail
}  // namespace net
}  // namespace muduo

using namespace muduo;
using namespace muduo::net;
using namespace muduo::net::detail;

TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),    //创建一个Timerfd,linux2.6.25版本新增的定时器接口，实际是一个fd
    timerfdChannel_(loop, timerfd_), //创建一个channl对象,加入io线程的EventLoop
    timers_(),                      //set(pair<Timestamp,Timer*>)
    callingExpiredTimers_(false)
{
  timerfdChannel_.setReadCallback(
      std::bind(&TimerQueue::handleRead, this));  //注册回调事件
  // we are always reading the timerfd, we disarm it with timerfd_settime.
  //IO多路复用中注册可读事件
  timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue()
{
  timerfdChannel_.disableAll();
  timerfdChannel_.remove();
  ::close(timerfd_);
  // do not remove channel, since we're in EventLoop::dtor();
  for (const Entry& timer : timers_)
  {
    delete timer.second;
  }
}

//线程安全的,可以跨线程调用
TimerId TimerQueue::addTimer(TimerCallback cb,  //用户自定义回调函数
                             Timestamp when,    //超时时间
                             double interval)   //间隔时间
{
  Timer* timer = new Timer(std::move(cb), when, interval);
  loop_->runInLoop(
      std::bind(&TimerQueue::addTimerInLoop, this, timer)); //注册回调函数
  return TimerId(timer, timer->sequence()); //唯一标识一个Timer
}

void TimerQueue::cancel(TimerId timerId)
{
  // loop_->runInLoop(
  //     std::bind(&TimerQueue::cancelInLoop, this, timerId));
  cancelInLoop(timerId);
}

void TimerQueue::addTimerInLoop(Timer* timer)
{
  //是否为当前线程
  loop_->assertInLoopThread();
  //插入一个定时器,有可能会使得最早到期的定时器发生改变
  bool earliestChanged = insert(timer);
  //earliestChanged:标志是否更改了最早的定时器
  if (earliestChanged)
  { 
    //重置定时器的超时时刻(timerfd_settime)
    resetTimerfd(timerfd_, timer->expiration());
  }
}


void TimerQueue::cancelInLoop(TimerId timerId)
{
  //断言在io线程中
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  ActiveTimer timer(timerId.timer_, timerId.sequence_);
  ActiveTimerSet::iterator it = activeTimers_.find(timer);
  //activeTimers_中找到了要删除的定时器
  if (it != activeTimers_.end())
  {
    size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
    assert(n == 1); (void)n;
    delete it->first; // FIXME: no delete please,使用unique_ptr,就不需要手动delete
    activeTimers_.erase(it);  //在activeTimers_和timers_中都删除定时器
  }
  //处理自己cancel的情况
  else if (callingExpiredTimers_)
  {
    //定时器已经到期了,并且正在进行handleRead()
    cancelingTimers_.insert(timer);
  }
  assert(timers_.size() == activeTimers_.size());
}

//Timerfd可读时触发该函数
void TimerQueue::handleRead()
{
  loop_->assertInLoopThread();
  Timestamp now(Timestamp::now());
  //读取超时时间,清楚该事件,避免一直触发
  readTimerfd(timerfd_, now);

  //获取当前时刻之前所有的定时器列表,即超时定时器列表
  std::vector<Entry> expired = getExpired(now);

  callingExpiredTimers_ = true;
  cancelingTimers_.clear();
  // safe to callback outside critical section
  for (const Entry& it : expired)
  {
    it.second->run(); //执行回调函数
  }
  callingExpiredTimers_ = false;

  //不是一次性定时器,需要重启
  reset(expired, now);
}

//rvo优化，return vector不会发生拷贝构造
//获取所有超时的定时器5
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
  assert(timers_.size() == activeTimers_.size());
  std::vector<Entry> expired;
  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  //寻找第一个没有超时的定时器
  //lower_bound返回第一个值>=sentry的元素的iterator
  //即*end >= sentry,因为sentry中Timer*最大,所以end->first > now
  TimerList::iterator end = timers_.lower_bound(sentry);
  
  assert(end == timers_.end() || now < end->first);
  //将到期的定时器插入到expired中
  std::copy(timers_.begin(), end, back_inserter(expired));
  //从timers_中国移除到期的定时器
  timers_.erase(timers_.begin(), end);

  //从activerTimers_中移除到期的定时器
  for (const Entry& it : expired)
  {
    ActiveTimer timer(it.second, it.second->sequence());
    size_t n = activeTimers_.erase(timer);
    assert(n == 1); (void)n;
  }

  assert(timers_.size() == activeTimers_.size());
  return expired;
}

void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
  Timestamp nextExpire;

  for (const Entry& it : expired)
  {
    ActiveTimer timer(it.second, it.second->sequence());
    //是重复的定时器并且是未取消的定时器,则重启该定时器
    if (it.second->repeat()
        && cancelingTimers_.find(timer) == cancelingTimers_.end())
    {
      it.second->restart(now);
      insert(it.second);
    }
    else
    {
      //一次性定时器 或者 已经被取消的定时器是不能重置的,因此删除该定时器
      // FIXME move to a free list
      delete it.second; // FIXME: no delete please
    }
  }

  if (!timers_.empty())
  {
    nextExpire = timers_.begin()->second->expiration();
  }

  //重新设置定时器属性
  if (nextExpire.valid())
  {
    resetTimerfd(timerfd_, nextExpire);
  }
}
//插入定时器
bool TimerQueue::insert(Timer* timer)
{
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  bool earliestChanged = false;
  Timestamp when = timer->expiration();
  //获取队列中定时时间最短的项,即第一个,因为数据结构是set,红黑树有序,比较顺序为pair的比较顺序
  TimerList::iterator it = timers_.begin();
  //timers_中不存在Timer或者定时时间小于最小的那一个
  if (it == timers_.end() || when < it->first)
  {
    earliestChanged = true; //说明插入的timers为第一个元素,即更新最小值.
  }
  {
    //插入到timers_中
    std::pair<TimerList::iterator, bool> result
      = timers_.insert(Entry(when, timer));
    assert(result.second); (void)result;
  }
  {
    //插入到activeTimers_中
    std::pair<ActiveTimerSet::iterator, bool> result
      = activeTimers_.insert(ActiveTimer(timer, timer->sequence())); 
    assert(result.second); (void)result;
  }
  assert(timers_.size() == activeTimers_.size());
  return earliestChanged;
}

