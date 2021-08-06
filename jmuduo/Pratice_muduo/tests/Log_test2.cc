#include<iostream>
#include<muduo/base/Logging.h>

using namespace muduo;

FILE *g_file;

void dummyOutput(const char *msg, int len)
{
    if(g_file)
    {
        fwrite(msg,1,len,g_file);
    }
}

void dunnmyFlush()
{
    fflush(g_file);
}

int main()
{
    g_file = ::fopen("/tmp/muduo_log", "ae");
    Logger::setOutput(dummyOutput);
    Logger::setFlush(dunnmyFlush);

    LOG_TRACE << "trace ...";
    LOG_DEBUG << "debug ...";
    LOG_INFO  << "info ...";
    LOG_WARN  << "warn ...";
    LOG_ERROR << "error ...";
    //LOG_FATAL << "fatal ...";

    errno = 13;
    LOG_SYSERR << "syserr ...";
    // << "sysfatal ...";

    ::fclose(g_file);
    return 0;
}