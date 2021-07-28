// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_ACCEPTOR_H
#define MUDUO_NET_ACCEPTOR_H

#include <functional>

#include "muduo/net/Channel.h"
#include "muduo/net/Socket.h"

namespace muduo
{
namespace net
{

class EventLoop;
class InetAddress;

///
/// Acceptor of incoming TCP connections.
///
class Acceptor : noncopyable
{
 public:
  typedef std::function<void (int sockfd, const InetAddress&)> NewConnectionCallback;

  Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { newConnectionCallback_ = cb; }

  void listen();

  bool listening() const { return listening_; }

  // Deprecated, use the correct spelling one above.
  // Leave the wrong spelling here in case one needs to grep it for error messages.
  // bool listenning() const { return listening(); }

 private:
  void handleRead();

  EventLoop* loop_;//acceptChannel_所属的EventLoop
  Socket acceptSocket_;//监听套接字
  Channel acceptChannel_;//该通道会观察acceptSocket_的可读事件
  NewConnectionCallback newConnectionCallback_;//accept连接后执行的回调函数
  bool listening_;//套接字是否处于监听状态
  int idleFd_;//处理文件描述符太多的情况
};

}  // namespace net
}  // namespace muduo

#endif  // MUDUO_NET_ACCEPTOR_H
