#include <iostream>

using namespace std;

class A
{
public:
    A(int ii):i(ii) { cout << "A::A()" << endl;}
    ~A() { cout << "A::~A()" << endl;}
    void print() { cout << "A::print(),i ="<< i << endl; }
//子类里面可以用,外人不可以用
    void set(int ii) { i = ii; }
private:
    int i;
};

//继承
//父类私有的东西在子类中是存在的,但是在子类中不可以碰
class B : public A
{
public:
    B() : A(50) { cout << "B::B()" << endl; }
    ~B() { cout << "B::~B()" << endl; }
    void f() { set(20); print(); }
};


int main()
{
    B b;
    b.print();
    b.set(10);
    b.print();
    b.f();
    return 0;
}
