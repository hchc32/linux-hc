// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMERQUEUE_H
#define MUDUO_NET_TIMERQUEUE_H

#include <set>
#include <vector>

#include "muduo/base/Mutex.h"
#include "muduo/base/Timestamp.h"
//#include "muduo/net/Callbacks.h"
#include "muduo/net/Channel.h"
#include "muduo/net/Timer.h"

namespace muduo
{
namespace net
{

class EventLoop;
class Timer;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
//定时器队列(事件驱动)
class TimerQueue : noncopyable
{
 public:
 //显示ctor
  explicit TimerQueue(EventLoop* loop);
  ~TimerQueue();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  
  //添加定时器
  //addTimer()->addTimerInLoop()->insert()->resetTimerfd()->timerfd_settime()
  //返回定时器的标号Id
  TimerId addTimer(TimerCallback cb,
                   Timestamp when,
                   double interval);

  //取消定时器
  void cancel(TimerId timerId);

 private:

  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  // This requires heterogeneous comparison lookup (N3465) from C++14
  // so that we can find an T* in a set<unique_ptr<T>>.
  
  //时间戳和定时器组成的pair,然后用set存储
  typedef std::pair<Timestamp, Timer*> Entry;
  typedef std::set<Entry> TimerList;

  //TimerList == ActiveTimerSet

  //定时器地址+序号
  typedef std::pair<Timer*, int64_t> ActiveTimer;
  typedef std::set<ActiveTimer> ActiveTimerSet;

  //以下成员函数只可能在其所属于的io线程中调用,因此不必加锁.
  void addTimerInLoop(Timer* timer);
  void cancelInLoop(TimerId timerId);
  
  // called when timerfd alarms
  //定时器触发回调
  void handleRead();
  
  // move out all expired timers
  //返回超时的定时器列表
  std::vector<Entry> getExpired(Timestamp now);
  
  //可重复的并且不希望删除的重新加入请求队列，其他的定时器删除
  void reset(const std::vector<Entry>& expired, Timestamp now);

  //插入一个定时器
  bool insert(Timer* timer);

  //事件循环
  EventLoop* loop_;
  //定时fd
  const int timerfd_;
  //定时器通道1
  Channel timerfdChannel_;

  // Timer list sorted by expiration
  //按到期时间排序的计时器列表
  TimerList timers_;

  // for cancel()
  //timers_与activeTimers_保存的是相同的数据
  //timers_是按到期时间排序,activeTimers_是按照对象地址排序的
  ActiveTimerSet activeTimers_;

  //是否处于处理定时器事件当中
  bool callingExpiredTimers_; /* atomic */

  //保存的是被取消的定时器
  ActiveTimerSet cancelingTimers_; 
};

}  // namespace net
}  // namespace muduo
#endif  // MUDUO_NET_TIMERQUEUE_H
