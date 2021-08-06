#include"Thread.h"
#include<iostream>
#include<unistd.h>
using namespace std;

class TestThread : public Thread
{
public:
    TestThread(int count):count_(count) 
    {
        cout << "TestThread ..." << endl;
    }

    ~TestThread() 
    {
        cout << "~TestThread ..." << endl;
    }
private:
    void Run()
    {
        while(count_--)
        {
            cout << "this is a test ..." << endl;
            sleep(1);
        }
    }
    int count_;
};

int main()
{
    // TestThread t(5);
    // t.Strat();
    // t.Join();
    TestThread *t2 = new TestThread(5);
    t2->SetAutoDelete(true);
    
    //子线程中运行
    t2->Strat();

    t2->Join();

    return 0;
}