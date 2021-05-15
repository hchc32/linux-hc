//左值可以出现在左侧
//右值只能出现在右侧
#include <iostream>
#include <complex>
#include <string>
using namespace std;

int main()
{
    int a = 9;
    int b = 4;
    a = b;
    b = a;
    a = a+b;
    //ERROR:a+b是右值
    //a+b=42;
    
    string s1("Hello");
    string s2("World");
    //s1+s2右值
    s1+s2=s1;
    cout << "s1:" << s1 << endl;
    cout << "s2:" << s2 << endl;
    //临时对象是右值
    string()="World";

    complex<int> c1(3,8),c2(1,0);
    c1+c2 = complex<int>(4,9);
    cout << "c1" << c1 << endl;
    cout << "c2" << c2 << endl;
    complex<int>() = complex<int>(4,9);
    return 0;
}

