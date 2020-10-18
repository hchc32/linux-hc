#include <iostream>

using namespace std;

class A
{
public:
    A():i(10) {}

    virtual void f() { cout << " A::f() i = " << i << endl; }
private:
    int i;
};

class B : public A
{
public:
    B():i(11) {}
    
    virtual void f() { cout << "B()::f() i = " << i << endl; }

private:
    int i;

};

int main()
{
    A a;
    B b;
    A *t = &a;
    int *p = (int*)&a; 
    int *q = (int*)&b;
    //a.f();
    //cout << sizeof(a) << endl;
    *p = *q;
    t->f();
    /*
    int *p = (int *)&a;
    int *q = (int *)&b;
    int *x = (int*)*p;
    cout << *p << " " << *q << endl;
    */
    //cout << x <<endl;
    return 0;
}
