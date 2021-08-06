#include "./AsyncLogging.h"

using namespace muduo;

AsyncLogging::AsyncLogging(const string &basename,
                           off_t rollSize,
                           int flushInterval)
    : flushInterval_(flushInterval),    //刷新间隔
      running_(false),                  //线程终止
      basename_(basename),              //文件名称
      rollSize_(rollSize),              //一个文件允许的最大字节数
      thread_(),
      latch_(),
      mutex_(),
      cond_(),
      currentBuffer_(),
      nextBuffer_(),
      buffers_()
{
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers.reserve(16);
}