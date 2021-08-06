#include<iostream>
#include<muduo/net/InetAddress.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/TcpServer.h>
#include<functional>

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
    }

    void start()
    {
        server_.start();
    }

private: 

    void onConnection(const TcpConnectionPtr& conn)
    {
        //处于连接状态
        if(conn->connected())
        {
            printf("User:onConnection(): new connection [%s] from %s\n", conn->name().c_str(),
                conn->peerAddress().toIpPort().c_str());
        }
        //连接断开状态
        else
        {
            printf("User:onConnection():connection [%s] is down\n", conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr& conn,const char* data,ssize_t len)
    {
        printf("onMessage():received %zd bytes from connection [%s]\n", len, conn->name().c_str());
        
    }

    EventLoop* loop_;
    TcpServer server_;
};


int main()
{
    printf("main(),pid = %d\n", getpid());

    InetAddress listenAddr(8888);
    EventLoop loop;
    TestServer server(&loop, listenAddr);
    server.start();
    loop.loop();
    return 0;
}