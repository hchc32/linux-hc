// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_THREADLOCAL_H
#define MUDUO_BASE_THREADLOCAL_H

#include "muduo/base/Mutex.h"
#include "muduo/base/noncopyable.h"

#include <pthread.h>

namespace muduo
{

template<typename T>
class ThreadLocal : noncopyable
{
 public:
  ThreadLocal()
  {
    //创建key
    MCHECK(pthread_key_create(&pkey_, &ThreadLocal::destructor));
  }

  ~ThreadLocal()
  {
    //销毁key,但是并不删除数据
    MCHECK(pthread_key_delete(pkey_));
  }

  //获取线程特定数据
  T& value()
  {
    T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
    //特定数据还没有被创建
    if (!perThreadValue)
    {
      T* newObj = new T();
      MCHECK(pthread_setspecific(pkey_, newObj));
      perThreadValue = newObj;
    }
    return *perThreadValue;
  }

 private:

  //pthread_key_create的回调函数,用于销毁堆上的数据
  static void destructor(void *x)
  {
    T* obj = static_cast<T*>(x);
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete obj;
  }

 private:
  pthread_key_t pkey_;  //key
};

}  // namespace muduo

#endif  // MUDUO_BASE_THREADLOCAL_H
