#include <iostream>
using namespace std;

namespace test1
{
    class A
    {
        public:
            int i;
            virtual void Print() {}
    };

    class B
    {
        public:
            int n;
            void Print() {}
    };

    void test_sizeof()
    {
        //16
        //4
        cout << "sizeof(A) = " <<sizeof(A) << endl\
             << "sizeof(B) = " <<sizeof(B) << endl;
        return;
    }
} // namespace test1



class A
{
public:
    virtual void vfunc1();
    virtual void vfunc2();
    void func1();
    void func2();

private:
    int m_data1;
    int m_data2;
};

class B:public A
{
public:
    virtual void vfunc1();
    void func2();

private:
    int m_data3;
};

class C:public B
{
public:
    virtual void vfunc1();
    void func2();

private:
    int m_data1;
    int m_data4;
};

int main()
{
    test1::test_sizeof();
    // //sizeof(point) = 8
    // cout << sizeof(A) << " ";
    // cout << sizeof(B) << " ";
    // cout << sizeof(C) << " ";
    return 0;
}
