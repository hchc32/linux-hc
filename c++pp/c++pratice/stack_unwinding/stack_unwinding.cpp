#include <iostream>
using namespace std;
 
// 函数f1会抛出一个异常
void f1() throw (int) 
{
    cout << "f1() Start " << endl;
    throw 100;
    cout << "f1() End " << endl;
}
 
// 调用函数f1
void f2() throw (int) 
{
    cout << "f2() Start " << endl;;
    f1();
    cout << "f2() End " << endl;
}
 
// 调用函数f2,并处理f1()抛上来的异常
void f3() 
{
    cout << "f3() Start " << endl;
    try 
    {
        f2();
    }
    catch (int i) 
    {
        cout << "Caught Exception: " << i << endl;
    }
    cout << "f3() End" << endl;
}
 
// 演示栈展开过程的程序
int main() 
{
    f3();
    return 0;
}