//构造函数
#include <iostream>

class A
{
public:
    int i;
    A();
    void f();
};

//构造函数
A::A()
{
    i = 0;
    printf("this=%p\n",this);
}

void A::f()
{
    this->i = 20;
    printf("A::f()--&i = %p\n",&i);
    printf("this=%p\n",this);
}

int main()
{
    A a;
    a.f();
    return 0;
}
