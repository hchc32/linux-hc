// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_LOGGING_H
#define MUDUO_BASE_LOGGING_H

#include "muduo/base/LogStream.h"
#include "muduo/base/Timestamp.h"

namespace muduo
{

class TimeZone;

//日志信息格式化输出到指定文件
class Logger
{
 public:
 //日志等级
  enum LogLevel
  {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  // compile time calculation of basename of source file
  class SourceFile  //处理传入的路径
  {
   public:

    template<int N>
    SourceFile(const char (&arr)[N])
      : data_(arr),
        size_(N-1)
    {
      const char* slash = strrchr(data_, '/'); // builtin function
      if (slash)
      {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
      }
    }

    explicit SourceFile(const char* filename)
      : data_(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        data_ = slash + 1;
      }
      size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
  };

  //ctor　impl
  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();
  
  LogStream& stream() { return impl_.stream_; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);

  typedef void (*OutputFunc)(const char* msg, int len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);
  static void setTimeZone(const TimeZone& tz);

 private:


class Impl
{
 public:
  typedef Logger::LogLevel LogLevel;
  //完成了每一条日志信息的时间，线程ID,日志等级的信息.
  //__thread的优化
  Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
  //负责日志的格式化
  void formatTime();
  
  void finish();

  Timestamp time_;    //时间戳:提供当前时间，可得到1970年到现在的毫秒数与生成标准时间字符串
  LogStream stream_;  //日志输出流
  LogLevel level_;    //当前日志级别
  int line_;          //日志行数 宏:__LINE__
  SourceFile basename_; //日志所属文件名
};

  Impl impl_;

};

extern Logger::LogLevel g_logLevel;

//获取当前的日志等级
inline Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}


//单例模式的log
template<typename Logger::LogLevel LEVEL>
class LoggerFactory : public noncopyable
{
private:
};

//
// CAUTION: do not write:
//
// if (good)
//   LOG_INFO << "Good news";
// else
//   LOG_WARN << "Bad news";
//
// this expends to
//
// if (good)
//   if (logging_INFO)
//     logInfoStream << "Good news";
//   else
//     logWarnStream << "Bad news";
//
#define LOG_TRACE if (muduo::Logger::logLevel() <= muduo::Logger::TRACE) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if (muduo::Logger::logLevel() <= muduo::Logger::DEBUG) \
  muduo::Logger(__FILE__, __LINE__, muduo::Logger::DEBUG, __func__).stream()
#define LOG_INFO if (muduo::Logger::logLevel() <= muduo::Logger::INFO) \
  muduo::Logger(__FILE__, __LINE__).stream()
#define LOG_WARN muduo::Logger(__FILE__, __LINE__, muduo::Logger::WARN).stream()
#define LOG_ERROR muduo::Logger(__FILE__, __LINE__, muduo::Logger::ERROR).stream()
#define LOG_FATAL muduo::Logger(__FILE__, __LINE__, muduo::Logger::FATAL).stream()
#define LOG_SYSERR muduo::Logger(__FILE__, __LINE__, false).stream() //ERROR级别
#define LOG_SYSFATAL muduo::Logger(__FILE__, __LINE__, true).stream() //FATAL级别

const char *
strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::muduo::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr)
{
  if (ptr == NULL)
  {
  //写一条日志然后终止程序
   Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

}  // namespace muduo

#endif  // MUDUO_BASE_LOGGING_H
