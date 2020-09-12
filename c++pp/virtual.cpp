#include <iostream>

using namespace std;

class A
{
public:
    A():i(10) {}

    virtual void f() { cout << " A::f() i = " << i << endl; }

    int i;
};

int main()
{
    A a,b;
    a.f();
    cout << sizeof(a) << endl;

    int *p = (int *)&a;
    int *q = (int *)&b;
    int *x = (int*)*p;
    cout << *p << " " << *q << endl;
    cout << x <<endl;
    return 0;
}
