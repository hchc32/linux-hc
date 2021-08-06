#include<muduo/net/EventLoop.h>
#include<muduo/base/Thread.h>
#include<iostream>
#include<unistd.h>

using namespace muduo;
using namespace muduo::net;

void threadFunc()
{
    printf("threadFunc():pid = %d,tid = %d\n",getpid(),CurrentThread::tid());

    EventLoop loop;
    loop.loop();
}

int main()
{
    printf("main():pid = %d,tid = %d\n",getpid(),CurrentThread::tid());
    EventLoop loop;

    muduo::Thread t(threadFunc);
    t.start();
    loop.loop();
    t.join();
    return 0;
}
