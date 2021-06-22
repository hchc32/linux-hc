#ifndef MUDUO_BASE_COUNTDOWNLATCH_H
#define MUDUO_BASE_COUNTDOWNLATCH_H

#include "./noncopyable.h"
#include "./Condition.h"
#include "./Mutex.h"

namespace muduo
{

class CountDownLatch : noncopyable
{

public:
    explicit CountDownLatch(int count);

    ~CountDownLatch();
    
};

}



#endif