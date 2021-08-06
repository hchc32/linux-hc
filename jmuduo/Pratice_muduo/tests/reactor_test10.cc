#include<muduo/net/EventLoopThreadPool.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/TcpServer.h>
#include<muduo/net/TcpConnection.h>
#include<muduo/net/EventLoop.h>

using namespace muduo::net;
using namespace muduo;

class TestServer
{
public:
    TestServer(EventLoop *loop,
               const InetAddress &listenAddr)
        : loop_(loop),
          server_(loop, listenAddr, "TestServer")
    {
        server_.setConnectionCallback(
            bind(&TestServer::onConnection, this, _1));
        server_.setMessageCallback(
            bind(&TestServer::onMessage, this, _1, _2, _3));
    }

    void setThreadNum(int threadNum)
    {
        server_.setThreadNum(threadNum);
    }

    void start()
    {
        server_.start();
    }

private:

    void onConnection(const TcpConnectionPtr& conn)
    {
        //连接建立
        if(conn->connected())
        {
            printf("onConnection(): new connection [%s] from %s \n",
                conn->name().c_str(), conn->peerAddress().toIpPort().c_str());
        
        }
        //连接断开
        else
        {
            printf("onConnection(): connection [%s] is down \n",
                conn->name().c_str());
        }
    }

    void onMessage(const TcpConnectionPtr &conn,
                   const char *data,
                   ssize_t len)
    {
        printf("onMessage():received %zd bytes from connection [%s]\n ",
                   len,conn->name().c_str());
    }

    EventLoop *loop_;
    TcpServer server_;
};

int main()
{
    printf("main():pid = %d\n", getpid());
    InetAddress listenAddr(8888);
    EventLoop loop;

    TestServer server(&loop,listenAddr);
    server.setThreadNum(4);
    server.start();
    loop.loop();

    return 0;
}