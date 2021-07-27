// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include "muduo/base/Atomic.h"
#include "muduo/base/Timestamp.h"
//#include "muduo/net/Callbacks.h"

#include<functional>

namespace muduo
{
namespace net
{

///
/// Internal class for timer event.
///
//内部封装一个定时器
//定时器--->回调函数+定时时间
typedef std::function<void()> TimerCallback;
class Timer : noncopyable
{
 public:
  Timer(TimerCallback cb, Timestamp when, double interval)
    : callback_(std::move(cb)),
      expiration_(when),
      interval_(interval),
      repeat_(interval > 0.0),
      sequence_(s_numCreated_.incrementAndGet()) //原子操作,先add后get
  { }

  //调用回调函数
  void run() const
  {
    callback_();
  }

  //获取定时器过期时间
  Timestamp expiration() const  { return expiration_; }
  //是否是周期性定时
  bool repeat() const { return repeat_; }
  //
  int64_t sequence() const { return sequence_; }

  //重启
  void restart(Timestamp now);

  static int64_t numCreated() { return s_numCreated_.get(); }

 private:
  //定时器回调函数
  const TimerCallback callback_;
  //定时器下一次的超时时刻
  Timestamp expiration_;
  //定时器超时时间间隔,如果是一次性定时器,该值为0
  const double interval_;
  //是否重复
  const bool repeat_;
  //定时器序号
  const int64_t sequence_;
  //当前已经创建的定时器数量
  static AtomicInt64 s_numCreated_;
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_TIMER_H
