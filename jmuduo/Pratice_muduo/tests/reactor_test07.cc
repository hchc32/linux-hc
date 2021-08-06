#include<muduo/net/EventLoop.h>
#include<muduo/net/Acceptor.h>
#include<muduo/net/InetAddress.h>
#include<muduo/net/SocketsOps.h>
#include<iostream>

using namespace muduo;
using namespace muduo::net;


void newConnection(int sockfd,const InetAddress& peerAddr)
{
    printf("newConnecton():accepted a new connection form %s\n", peerAddr.toIpPort().c_str());
    ::write(sockfd, "How are you?\n", 13);
    sockets::close(sockfd);
}

int main()
{
    printf("main(), pid = %d \n", getpid());

    InetAddress listenAddr(8888);
    EventLoop loop;
    Acceptor acceptor(&loop, listenAddr,true);
    acceptor.setNewConnectionCallback(newConnection);
    acceptor.listen();

    loop.loop();
    return 0;
}
