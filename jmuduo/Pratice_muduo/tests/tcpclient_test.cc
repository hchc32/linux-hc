#include<muduo/net/TcpClient.h>
#include<muduo/net/EventLoop.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/Channel.h>
#include<muduo/net/TcpConnection.h>
#include<muduo/base/Logging.h>

using namespace muduo;
using namespace muduo::net;

class TestClient
{
public:
    TestClient(EventLoop *loop, const InetAddress &listenAddr)
        : loop_(loop),
          client_(loop, listenAddr, "TestClient"),
          stdinChannel_(loop, 0)
    {
        client_.setConnectionCallback(
            std::bind(&TestClient::onConnection, this, _1));
        client_.setMessageCallback(
            std::bind(&TestClient::onMessage, this, _1, _2, _3));
        stdinChannel_.setReadCallback(bind(&TestClient::handleRead, this,_1));
        stdinChannel_.enableReading();
    }
    
    void connect()
    {
        client_.connect();
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

    void onMessage(const TcpConnectionPtr &conn,
                   Buffer *buf,
                   Timestamp receiveTime)
    {
        std::string msg(buf->retrieveAllAsString());
        printf("onMessage():recv a message [%s]\n", msg.c_str());
        LOG_TRACE << conn->name() << "recv" << msg.size() << "bytes at" << receiveTime.toFormattedString();
    }

    void handleRead(Timestamp receiveTime)
    {
        char buf[1024] = {0};
        fgets(buf, 1024, stdin);
        buf[strlen(buf) - 1] = '\0';
        client_.connection()->send(buf);
    }

    EventLoop *loop_;
    TcpClient client_;
    Channel stdinChannel_;
};

int main()
{
    LOG_INFO << "pid = " << getpid() << ", tid = " << CurrentThread::tid();

    EventLoop loop;
    InetAddress serverAddr("127.0.0.1", 8888);
    TestClient client(&loop, serverAddr);
    client.connect();
    loop.loop();
    
    return 0;
}