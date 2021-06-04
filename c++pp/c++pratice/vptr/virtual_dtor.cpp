#include<iostream>
using namespace std;

// 基类
class A 
{
public: 
    A()  // 构造函数
    {
        cout << "construct A" << endl;
    }
    
    virtual ~A() // 析构函数
    {
        cout << "Destructor A" << endl;
    }
};

// 派生类
class B : public A 
{
public: 
    B()  // 构造函数
    {
        cout << "construct B" << endl;
    }
    
    ~B()// 析构函数
    {
        cout << "Destructor B" << endl;
    }
};

int main() 
{
    A *pa = new B();
    delete pa;
    return 0;
}
//construct A
//construct B
//Destructor A
