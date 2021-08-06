#include<iostream>
#include<muduo/net/InetAddress.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/TcpServer.h>
#include<muduo/net/Buffer.h>
#include<functional>
#include <assert.h>

using namespace muduo;
using namespace muduo::net;


class TestServer
{
public:
    TestServer(EventLoop *loop,
               const InetAddress &listenAddr)
        : loop_(loop),
          server_(loop,listenAddr,"TestServer")
    {
        server_.setConnectionCallback(
            std::bind(&TestServer::onConnection, this, _1));
        server_.setMessageCallback(
            std::bind(&TestServer::onMessage, this, _1, _2, _3));
        message1_.resize(100);
        message2_.resize(200);
        std::fill(message1_.begin(), message1_.end(), 'A');
        std::fill(message2_.begin(),message2_.end(),'B');
    }

    void start()
    {
        server_.start();
    }

    void setThreadNum(int threadNum)
    {
        server_.setThreadNum(threadNum);
    }

private: 

    void onConnection(const TcpConnectionPtr& conn)
    {
        //处于连接状态
        if(conn->connected())
        {
            printf("User:onConnection(): new connection [%s] from %s\n", conn->name().c_str(),
                conn->peerAddress().toIpPort().c_str());
            conn->send(message1_);
            conn->send(message2_);
            conn->shutdown();
        }
        //连接断开状态
        else
        {
            printf("User:onConnection():connection [%s] is down\n", conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buf,
                   Timestamp receiveTime)
    {
        string msg(buf->retrieveAllAsString());
        printf("onMessage():received %zd bytes from connection [%s] at %s\n",
               msg.size(),
               conn->name().c_str(),
               receiveTime.toFormattedString().c_str());
        conn->send(msg);
    }

    EventLoop* loop_;
    TcpServer server_;
    muduo::string message1_;
    muduo::string message2_;
};

int main()
{
    printf("main(),pid = %d\n", getpid());

    InetAddress listenAddr(8888);
    EventLoop loop;
    
    TestServer server(&loop, listenAddr);
    server.setThreadNum(5);
    server.start();

    loop.loop();
    return 0;
}