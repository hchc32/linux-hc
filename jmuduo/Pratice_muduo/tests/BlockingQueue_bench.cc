#include<iostream>
#include<muduo/base/BlockingQueue.h>
#include<muduo/base/Thread.h>
#include<muduo/base/CountDownLatch.h>
#include<boost/ptr_container/ptr_vector.hpp>
#include<muduo/base/Timestamp.h>
#include<map>
#include<boost/bind/bind.hpp>

using namespace std;
using namespace boost::placeholders;
using namespace muduo;

class Bench
{
public:
    Bench(int numThreads):
        latch_(numThreads),
            threads_(numThreads)
    {
        for (int i=0;i<numThreads;i++)
        {
            char name[32];
            snprintf(name,sizeof(name),"work thread %d",i);
            threads_.push_back(new muduo::Thread(bind(&Bench::threadFunc,this),muduo::string(name)));
        }
        //进入回调函数
        for_each(threads_.begin(),threads_.end(),boost::bind(&muduo::Thread::start,_1));
    }

    void run(int times)
    {
        //等待所有线程初始化完成...
        printf("waiting for count down latch\n");
        latch_.wait();
        printf("all threads started");
        for (int i=0;i<times;i++)
        {
            muduo::Timestamp now(muduo::Timestamp::now());
            queue_.put(now);
            usleep(1000);
        }
    }

    void joinAll()
    {
        for (size_t i=0;i<threads_.size();i++)
        {
            queue_.put(muduo::Timestamp::invalid());
        }
        for_each(threads_.begin(),threads_.end(),bind(&muduo::Thread::join,_1));
    }

private:

    //回调函数
    void threadFunc()
    {
        printf("tid = %d,%s started\n",
               muduo::CurrentThread::tid(),
               muduo::CurrentThread::name());
        std::map<int, int> delays;
        latch_.countDown();
        bool running = true;
        while(running)
        {
            muduo::Timestamp t(queue_.take());
            muduo::Timestamp now(muduo::Timestamp::now());
            if(t.valid())
            {
                int delay = static_cast<int>(timeDifference(now, t) * 1000000);
                ++delays[delay];
            }
            running = t.valid();
        }
        printf("tid = %d,%s stopped\n",
               muduo::CurrentThread::tid(),
               muduo::CurrentThread::name());
        for (auto &x : delays)
        {
            printf("tid = %d,delay = %d,count = %d\n",
                   muduo::CurrentThread::tid(), x.first, x.second);
        }
    }

    muduo::BlockingQueue<muduo::Timestamp> queue_;
    muduo::CountDownLatch latch_;
    boost::ptr_vector<muduo::Thread> threads_;
};

int main(int argc,char* argv[])
{
    int threads = argc > 1 ? atoi(argv[1]) : 1;

    Bench t(threads);
    t.run(10000);
    t.joinAll();
    return 0;
}