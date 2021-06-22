#include "./Thread.h"
#include <cassert>
namespace muduo
{
namespace detail
{

struct ThreadData
{
    typedef muduo::Thread::ThreadFunc ThreadFunc;
    ThreadFunc func_;
    std::string name_;
    pid_t *tid_;
    CountDownLatch *latch_;

    ThreadData(ThreadFunc func,
               const string& name,
               pid_t *tid,
               CountDownLatch* latch)
        :func_(std::move(func)),
         name_(name),
         tid_(tid),
         latch_(latch)
    { }

    void runInThread()
    {
        *tid_ = muduo::CurrentThread::tid();
        tid_ = NULL;
        latch_->countDown();
        latch_ = NULL;

        try
        {
            func_();
        }
        catch(const std::exception& e)
        {
            //std::cerr << e.what() << '\n';
        }
        catch(...)
        {

        }  
    }

    void* startThread(void *obj)
    {
        ThreadData *data = static_cast<ThreadData *>(obj);
        data->runInThread();
        delete data;
        return NULL;
    }
};
} //namespace detail





Thread::Thread(ThreadFunc func, const std::string &n)
    : start_(false),
        joined_(false),
        pthreadId_(0),
        tid_(0),
        func_(std::move(func)),
        name_(n)
        //latch_(1)

{
    setDefaultName();
}

Thread::~Thread()
{
    //已启动没有被回收的线程
    if(start_ && !joined_)
    {
        pthread_detach(pthreadId_);
    }
}

void Thread::setDefaultName()
{
    int num = numCreated();
    num++;
    if(name_.empty())
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}

int Thread::join()
{
    assert(start_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

void Thread::start()
{
    assert(!start_);
    start_ = true;

}
} //namespace muduo