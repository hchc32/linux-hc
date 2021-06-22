#ifndef MUDUO_LOGGING_ASYNLOGGING_H
#define MUDUO_LOGGING_ASYNLOGGING_H

#include "../base/noncopyable.h"
#include <vector>
#include <array>
#include <memory>
#include <string>
namespace muduo
{

//异步日志
class AsyncLogging : noncopyable
{

public:
    AsyncLogging(const string& basename,
                 off_t rollSize,
                 int flushInterval);

    ~AsyncLogging()
    {
        if(running_)
        {
            stop();
        }
    }

    void append(const char *logline, int len);
    void start()
    {
        running_ = true;
        thread_.start();
        latch_.wait();
    }
    void stop()
    {
        running_ = false;
        cond_.notify();
        thread_.join();
    }

private:
    //线程回调函数
    void threadFunc();
    typedef std::array<char, 80> Buffer;
    typedef std::vector<std::unique_ptr<Buffer>> BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_; //线程是否开启
    const std::string basename_;
    const off_t rollSize_;

    muduo::Thread thread_;
    muduo::CountDownLatch latch_;
    muduo::MutexLock mutex_;
    muduo::Condition cond_;

    BufferPtr currentBuffer_;   //当前缓冲区
    BufferPtr nextBuffer_;      //预备缓冲区
    BufferVector buffers_;      //待写入文件的已填满的缓冲
};
}






#endif 