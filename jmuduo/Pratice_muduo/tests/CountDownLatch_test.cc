#include<iostream>
#include<functional>
#include<string>
#include<muduo/base/CountDownLatch.h>
#include<muduo/base/Thread.h>
#include<boost/ptr_container/ptr_vector.hpp>
#include<boost/bind/bind.hpp>

using namespace boost::placeholders;
using namespace muduo;
using namespace std;

class Test
{
public:
    Test(int numThreads)
        : latch_(1),
          threads_(numThreads)
    {
        for (int i=0;i<numThreads;i++)
        {
            char name[32];
            snprintf(name,sizeof(name),"work thread %d",i);
            threads_.push_back(new Thread(bind(&Test::threadFunc,this),muduo::string(name)));
        }
        //
        for_each(threads_.begin(), threads_.end(), bind(&Thread::start, _1));
    }

    void run()
    {
        latch_.countDown();
    }

    void joinAll()
    {
        for_each(threads_.begin(),threads_.end(),bind(&Thread::join,_1));
    }

private:

    void threadFunc()
    {
        latch_.wait();
        printf("tid = %d,%s started\n",\
                CurrentThread::tid(),\
                CurrentThread::name());

        printf("tid = %d,%s stopped\n",\
                CurrentThread::tid(),\
                CurrentThread::name());
    }

    CountDownLatch latch_;
    boost::ptr_vector<Thread> threads_;
};

int main()
{
    printf("pid = %d,tid = %d\n", ::getpid(), CurrentThread::tid());
    Test t(3);
    sleep(3);
    printf("pid = %d,tid = %d,%s running\n", ::getpid(), CurrentThread::tid(), CurrentThread::name());
    t.run();
    t.joinAll();

    printf("number of created threads %d\n", Thread::numCreated());

    return 0;
}