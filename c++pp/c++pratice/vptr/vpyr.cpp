#include <iostream>
using namespace std;

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
    //sizeof(point) = 8
    cout << sizeof(A) << " ";
    cout << sizeof(B) << " ";
    cout << sizeof(C) << " ";
    return 0;
}
