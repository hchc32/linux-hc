// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "muduo/net/TcpServer.h"

#include "muduo/base/Logging.h"
#include "muduo/net/Acceptor.h"
#include "muduo/net/EventLoop.h"
#include "muduo/net/EventLoopThreadPool.h"
#include "muduo/net/SocketsOps.h"

#include <stdio.h>  // snprintf

using namespace muduo;
using namespace muduo::net;

TcpServer::TcpServer(EventLoop* loop,
                     const InetAddress& listenAddr,
                     const string& nameArg,
                     Option option)
  : loop_(CHECK_NOTNULL(loop)),//Logging.h中定义,检测loop_是否为空
    ipPort_(listenAddr.toIpPort()),//监听套接字的端口号
    name_(nameArg),//TcpServer名称
    acceptor_(new Acceptor(loop, listenAddr, option == kReusePort)),
    threadPool_(new EventLoopThreadPool(loop, name_)), //loop->mainreactor
    connectionCallback_(defaultConnectionCallback),
    messageCallback_(defaultMessageCallback),
    nextConnId_(1)
{
  //Acceptor::handleRead()函数会回调TcpServer::newCOnnection
  //_1对应的是socket文件描述符，_2对应的是对等方的地址(InetAddress)
  acceptor_->setNewConnectionCallback(
      std::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
  loop_->assertInLoopThread();
  LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

  for (auto& item : connections_)
  {
    TcpConnectionPtr conn(item.second);
    item.second.reset();
    conn->getLoop()->runInLoop(
      std::bind(&TcpConnection::connectDestroyed, conn));
  }
}

void TcpServer::setThreadNum(int numThreads)
{
  assert(0 <= numThreads);
  threadPool_->setThreadNum(numThreads);
}

//可以跨线程调用
void TcpServer::start()
{
  if (started_.getAndSet(1) == 0)
  {
    //启动线程池
    threadPool_->start(threadInitCallback_);

    //断言是否处于监听状态
    assert(!acceptor_->listening());
    loop_->runInLoop(
        std::bind(&Acceptor::listen, get_pointer(acceptor_)));
        //get_pointer返回智能指针的原生指针.
  }
}

//TcpServer::newConnctiopn() -> <<create>>TcpConnection 
// -> connectEstablished()   -> connCb()
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
  //断言在io线程中
  loop_->assertInLoopThread();
  //按照轮询的方式选择一个EventLoop
  EventLoop* ioLoop = threadPool_->getNextLoop();

  char buf[64];
  snprintf(buf, sizeof buf, "-%s#%d", ipPort_.c_str(), nextConnId_);
  ++nextConnId_;
  //当前连接的名称
  string connName = name_ + buf;

  LOG_INFO << "TcpServer::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << peerAddr.toIpPort();
  //构造一个本地地址
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  //创建一个TcpConnection对象,立即用shared_ptr来接管
  TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
  //LOG_TRACE << "[1] usecount=" << conn.use_count();//1
  //将连接对象放在map容器中
  connections_[connName] = conn;
  //LOG_TRACE << "[2] usecount=" << conn.use_count();//2

  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(
      std::bind(&TcpServer::removeConnection, this, _1)); // FIXME: unsafe
  ioLoop->runInLoop(std::bind(&TcpConnection::connectEstablished, conn));
  //LOG_TRACE << "[5] usecount=" << conn.use_count(); //2
  /* newConnection结束之后,conn就会使得引用计数减一,变为１,只有connections_ map 中有*/
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
  // FIXME: unsafe
  //LOG_TRACE << "[8.1] usecount=" << conn.use_count();//传递this指针,造成引用计数+1
  loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
  loop_->assertInLoopThread();
  //LOG_TRACE << "[8.2] usecount=" << conn.use_count();//
  LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
           << "] - connection " << conn->name();
  //LOG_TRACE << "[8] usecount=" << conn.use_count();//4
  size_t n = connections_.erase(conn->name());
  //LOG_TRACE << "[9] usecount=" << conn.use_count();//3
  (void)n;
  assert(n == 1);
  EventLoop* ioLoop = conn->getLoop();
  ioLoop->queueInLoop(
      std::bind(&TcpConnection::connectDestroyed, conn)); //loop中有一个shared_ptr
  //LOG_TRACE << "[10] usecount=" << conn.use_count();//4
}

