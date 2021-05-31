#include<iostream>
using namespace std;

// 基类
class CFather 
{
public:
    virtual void hello() // 虚函数
    {
        cout<<"hello from father"<<endl; 
    }
    
    virtual void bye() // 虚函数
    {
        cout<<"bye from father"<<endl; 
    }
};

// 派生类
class CSon : public CFather
{ 
public:
    CSon() // 构造函数
    { 
        hello(); 
    }
    
    ~CSon()  // 析构函数
    { 
        bye();
    }

    virtual void hello() // 虚函数
    { 
        cout<<"hello from son"<<endl;
    }
    virtual void bye()
    {
        cout << "bye from son" << endl;
    }
};

int main()
{
    CSon son;
    CFather *pfather;
    pfather = & son;
    pfather->hello(); //多态
    return 0;
    //hello from son  // 构造son对象时执行的构造函数
    //hello from son  // 多态
    //bye from father // son对象析构时，由于CSon类没有bye成员函数，所以调用了基类的bye成员函数
}
