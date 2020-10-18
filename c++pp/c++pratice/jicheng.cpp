#include <iostream>
#include<string>
using namespace std;

class A
{
public:
    int i;
public:
    A():i(10) {}
};

class B: public A\
{
private:
    int j;
public:
    B():j(13) {}
    void f() {cout << "B.j=" << j <<endl;}
};

int main()
{
    A a;
    B b;
    cout << a.i << " " << b.i <<endl;
    cout << sizeof(a) << " " << sizeof(b) << endl;

    int *p = (int*)&a;
    cout << p << " " << *p << endl;
    *p = 20;
    cout << "a.i = " << a.i << endl;

    int *q = (int*)&b;
    cout << q << " " << *q << endl; 
    q++;
    *q = 50;
    b.f();
    return 0;
}
