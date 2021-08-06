// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/base/Logging.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"

#include <sstream>

#include <poll.h>

using namespace muduo;
using namespace muduo::net;

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;//PRI表示紧急数据(tcp带外数据)
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fd__)
  : loop_(loop),
    fd_(fd__),
    events_(0),
    revents_(0),
    index_(-1),
    logHup_(true),
    tied_(false),
    eventHandling_(false),
    addedToLoop_(false)
{
}

Channel::~Channel()
{
  assert(!eventHandling_);
  assert(!addedToLoop_);
  if (loop_->isInLoopThread())
  {
    assert(!loop_->hasChannel(this));

  }
}

void Channel::tie(const std::shared_ptr<void>& obj)
{
  tie_ = obj;
  tied_ = true;
}

void Channel::update()
{
  addedToLoop_ = true;
  loop_->updateChannel(this);
}

void Channel::remove()
{
  assert(isNoneEvent());
  addedToLoop_ = false;
  loop_->removeChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime)
{
  std::shared_ptr<void> guard;
  if (tied_)
  {
    guard = tie_.lock();//弱引用提升为shared_ptr
    if (guard)
    {
      //LOG_TRACE << "[6] usecount=" << guard.use_count();//2
      handleEventWithGuard(receiveTime);
      //LOG_TRACE << "[12] usecount=" << guard.use_count();//2
    }
  }
  else
  {
    handleEventWithGuard(receiveTime);
  }
  LOG_TRACE << "[13] usecount=" << guard.use_count();//2
  //函数结束之后use_count=1
}

void Channel::handleEventWithGuard(Timestamp receiveTime)
{
  eventHandling_ = true;
  // if(revents_ & POLLHUP)
  // {
  //   LOG_TRACE << "1111111111111111";
  // }
  // if(revents_ & POLLIN)
  // {
  //   LOG_TRACE << "22222222222222222";
  // }
  LOG_TRACE << reventsToString();
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
  {
    if (logHup_)
    {
      LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLHUP";
    }
    if (closeCallback_) closeCallback_();
  }

  //fd没有打开.
  if (revents_ & POLLNVAL)
  {
    LOG_WARN << "fd = " << fd_ << " Channel::handle_event() POLLNVAL";
  }

  if (revents_ & (POLLERR | POLLNVAL))
  {
    if (errorCallback_) errorCallback_();
  }
  //POLLRDHUP:对端关闭连接
  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if (readCallback_) readCallback_(receiveTime);
  }
  //可写事件
  if (revents_ & POLLOUT)
  {
    if (writeCallback_) writeCallback_();
  }
  eventHandling_ = false;
}

string Channel::reventsToString() const
{
  return eventsToString(fd_, revents_);
}

string Channel::eventsToString() const
{
  return eventsToString(fd_, events_);
}

string Channel::eventsToString(int fd, int ev)
{
  std::ostringstream oss;
  oss << fd << ": ";
  if (ev & POLLIN)
    oss << "IN ";
  if (ev & POLLPRI)
    oss << "PRI ";
  if (ev & POLLOUT)
    oss << "OUT ";
  if (ev & POLLHUP)
    oss << "HUP ";
  if (ev & POLLRDHUP)
    oss << "RDHUP ";
  if (ev & POLLERR)
    oss << "ERR ";
  if (ev & POLLNVAL)
    oss << "NVAL ";

  return oss.str();
}
