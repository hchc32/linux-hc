
//private是对类来说的，同一个类的不同对象之间的私有成员是可以访问的

#include <iostream>

using namespace std;

class A
{
private:
    int i;
    int* p;
public:
    A() { p = 0; cout << "A::A()" << endl; }
    ~A() {if (p) delete p ; cout << "A::~A(),i ="<< i << endl; }
    void set(int i) {this->i = i;}
    void f() {p = new int;}
    void g(A* q) {cout <<"A::g(),q->i ="<< q->i << endl ;}
};

int main()
{
    A* p = new A[10];
    for(int i = 0 ; i < 10 ; i++)
    {
        p[i].set(i);
    }
    A b;
    b.set(100);
    p->g(&b);
    delete [] p;
    return 0;
}
