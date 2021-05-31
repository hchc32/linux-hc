#include<iostream>
using namespace std;

namespace test1
{
    //基类
    class CFather
    {
        public:
            virtual void fun() { cout <<  "father" << endl; };
    };

    //派生类
    class CSon:public CFather
    {
        public:
            virtual void fun() { cout << "son" << endl; };
    };

    void test()
    {
        CSon son;
        //基类指针指向派生类
        CFather *p = &son;
        //通过基类指针调用派生类的函数fun()
        p->fun();
        return;
    }
} // namespace test1

namespace test2
{
        //基类
    class CFather
    {
        public:
            virtual void fun() { cout <<  "father" << endl; };
    };

    //派生类
    class CSon:public CFather
    {
        public:
            virtual void fun() { cout << "son" << endl; };
    };

    void test()
    {
        CSon  son;
        CFather &r = son;
        r.fun();
        return;
    }
} // namespace test2

int main()
{
    //基类的指针访问派生类
    test1::test();
    //基类的引用访问派生类
    test2::test();
    return 0;
}