#include"Thread.h"
#include<iostream>
#include<unistd.h>
using namespace std;

Thread::Thread():autoDelete_(false)
{
    cout << "Thread ..." << endl;
}

Thread::~Thread() 
{
    cout << "~Thread ..." << endl;
}

//线程启动函数
void Thread::Strat()
{
    pthread_create(&threadId_,NULL,ThreadRoutine,this);
}

//线程回收函数
void Thread::Join()
{
    cout << pthread_self() << endl;
    pthread_join(threadId_, NULL);
}

//线程回调函数
void *Thread::ThreadRoutine(void *arg)
{
    Thread *thread = static_cast<Threa d*>(arg);
    thread->Run();
    cout <<"run"<< pthread_self() << endl;
    //线程逻辑执行结束后,结束该线程.
    if(thread->autoDelete_)
    {
        delete thread;
    }
    return NULL;
}

void Thread::SetAutoDelete(bool autoDelete)
{
    autoDelete_ = autoDelete;
}
