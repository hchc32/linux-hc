#include "Thread1.h"
#include <iostream>
#include <unistd.h>
#include <functional>
using namespace std;

class Foo
{
public:
    Foo(int count) : count_(count)
    {
    }

    void MemberFun()
    {
        while (count_--)
        {
            cout << "this is a test ..." << endl;
            sleep(1);
        }
    }

    void MemberFun2(int p)
    {
        while (count_--)
        {
            cout << p << "=this is a test ..." << endl;
            sleep(1);
        }
    }

private:
    int count_;
};

void ThreadFunc()
{
    cout << "ThreadFunc..." << endl;
}

void ThreadFunc2(int count)
{
    cout << count << "ThreadFunc..." << endl;
}

int main()
{
    Thread t1(ThreadFunc);
    Thread t2(bind(ThreadFunc2, 3));
    Foo foo(3);
    Thread t3(bind(&Foo::MemberFun, &foo));
    Foo foo2(3);
    Thread t4(bind(&Foo::MemberFun2, &foo2, 1000)); 
    t1.Strat();
    t2.Strat();
    t3.Strat();
    t4.Strat();

    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();
    return 0;
}