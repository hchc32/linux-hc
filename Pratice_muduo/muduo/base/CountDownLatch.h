#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include "./noncopyable.h"
#include "./Condition.h"
#include "./Mutex.h"

namespace muduo
{

//Condition + Count
class CountDownLatch : noncopyable
{

public:
    explicit CountDownLatch(int count);

    ~CountDownLatch() = default;

    void wait();

    void countDown();

    int getCount() const;

private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};

} // namespace muduo

#endif