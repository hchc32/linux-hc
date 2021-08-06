// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include "muduo/net/Connector.h"

#include "muduo/base/Logging.h"
#include "muduo/net/Channel.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/SocketsOps.h"

#include <errno.h>

using namespace muduo;
using namespace muduo::net;

const int Connector::kMaxRetryDelayMs;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
  : loop_(loop),
    serverAddr_(serverAddr),
    connect_(false),
    state_(kDisconnected),
    retryDelayMs_(kInitRetryDelayMs)
{
  LOG_DEBUG << "ctor[" << this << "]";
}

Connector::~Connector()
{
  LOG_DEBUG << "dtor[" << this << "]";
  assert(!channel_);
}

//可以跨线程调用
void Connector::start()
{
  connect_ = true;
  loop_->runInLoop(std::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::startInLoop()
{
  //断言处于io线程中
  loop_->assertInLoopThread();
  //断言状态是未连接
  assert(state_ == kDisconnected);
  if (connect_)
  {
    connect();
  }
  else
  {
    LOG_DEBUG << "do not connect";
  }
}

void Connector::stop()
{
  connect_ = false;
  loop_->queueInLoop(std::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
  // FIXME: cancel timer
}

void Connector::stopInLoop()
{
  loop_->assertInLoopThread();
  if (state_ == kConnecting)//处于连接状态
  {
    setState(kDisconnected);
    int sockfd = removeAndResetChannel(); //将通道从poller中移除,并将channel置空
    retry(sockfd);//并不要重连,只是调用close(sockfd)
  }
}

void Connector::connect()
{
  //创建一个非阻塞套接字
  int sockfd = sockets::createNonblockingOrDie(serverAddr_.family());
  int ret = sockets::connect(sockfd, serverAddr_.getSockAddr());
  int savedErrno = (ret == 0) ? 0 : errno;
  switch (savedErrno)
  {
    case 0:
    case EINPROGRESS://非阻塞套接字,未连接成功返回码是EINPROGRESS表示正在连接
    case EINTR:
    case EISCONN://连接成功
      connecting(sockfd);
      break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH: //重连
      retry(sockfd);
      break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      break;

    default:
      LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      // connectErrorCallback_();
      break;
  }
}

//重启
void Connector::restart()
{
  loop_->assertInLoopThread();
  setState(kDisconnected);
  retryDelayMs_ = kInitRetryDelayMs;
  connect_ = true;
  startInLoop();
}

void Connector::connecting(int sockfd)
{
  setState(kConnecting);
  assert(!channel_);
  //创建一个Channel对象
  channel_.reset(new Channel(loop_, sockfd));
  //设置可写回调函数,这时候如果socket没有错误,sockfd就处于可写状态
  channel_->setWriteCallback(
      std::bind(&Connector::handleWrite, this)); // FIXME: unsafe
  //设置错误回调函数
  channel_->setErrorCallback(
      std::bind(&Connector::handleError, this)); // FIXME: unsafe

  // channel_->tie(shared_from_this()); is not working,
  // as channel_ is not managed by shared_ptr
  //关注可写事件
  channel_->enableWriting();
}

int Connector::removeAndResetChannel()
{
  //不关注任何事件了
  channel_->disableAll();
  channel_->remove();
  int sockfd = channel_->fd();
  // Can't reset channel_ here, because we are inside Channel::handleEvent
  //不能在这里重置channel_,因为正在调用Channel::handleEvent
  loop_->queueInLoop(std::bind(&Connector::resetChannel, this)); // FIXME: unsafe
  return sockfd;
}

//置空channel
void Connector::resetChannel()
{
  channel_.reset();
}

void Connector::handleWrite()
{
  LOG_TRACE << "Connector::handleWrite " << state_;

  if (state_ == kConnecting)
  {
    int sockfd = removeAndResetChannel();//从poller中移除关注,并将channel置空
    //sockfd可写并不意味这连接一定建立成功,还需要用getsockopt再次确认一下
    int err = sockets::getSocketError(sockfd);
    if (err) //有错误
    {
      LOG_WARN << "Connector::handleWrite - SO_ERROR = "
               << err << " " << strerror_tl(err);
      retry(sockfd);//重连
    }
    else if (sockets::isSelfConnect(sockfd)) //自连接？
    {
      LOG_WARN << "Connector::handleWrite - Self connect";
      retry(sockfd);//重连
    }
    else //连接成功
    {
      //更改状态
      setState(kConnected);
      if (connect_)
      {
        //回调函数
        newConnectionCallback_(sockfd);
      }
      else
      {
        //关闭套接字
        sockets::close(sockfd);
      }
    }
  }
  else
  {
    // what happened?
    assert(state_ == kDisconnected);
  }
}

//错误回调函数
void Connector::handleError()
{
  LOG_ERROR << "Connector::handleError state=" << state_;
  if (state_ == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
    retry(sockfd);
  }
}

//重连,使用back-off策略重连,重连时间逐渐延长,0.5s 1s 2s ...直到30s
void Connector::retry(int sockfd)
{
  sockets::close(sockfd);
  setState(kDisconnected);
  if (connect_)
  {
    LOG_INFO << "Connector::retry - Retry connecting to " << serverAddr_.toIpPort()
             << " in " << retryDelayMs_ << " milliseconds. ";
    //注册一个定时操作,重连
    loop_->runAfter(retryDelayMs_/1000.0,
                    std::bind(&Connector::startInLoop, shared_from_this()));
    retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
  }
  else
  {
    LOG_DEBUG << "do not connect";
  }
}

