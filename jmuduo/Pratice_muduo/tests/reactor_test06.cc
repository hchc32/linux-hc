#include<iostream>
#include<muduo/base/CurrentThread.h>
#include<muduo/net/EventLoopThread.h>
#include<muduo/net/EventLoop.h>
#include<unistd.h>
using namespace muduo;
using namespace muduo::net;

void runInThread()
{
    printf("runInLoop():pid = %d,tid = %d\n", getpid(), CurrentThread::tid());
}

int main()
{
    printf("main(),pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
    EventLoopThread loopThread;
    EventLoop *loop = loopThread.startLoop();

    //异步调用runInThread,即将runInLoop添加到loop对象所在的io线程,让io线程去执行
    loop->runInLoop(runInThread);
    sleep(1);

    //runAfter内部也调用了runInLoop,这里也是异步调用
    loop->runAfter(2,runInThread);
    sleep(3);
    loop->quit();

    printf("exit main().\n");
}