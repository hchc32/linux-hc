#include <iostream>

using namespace std;
class A
{
public:
        void f();
        int i;
};

void A::f()
{
    i = 20;
    if(i>10)
      cout << i << endl;
}

//函数是属于类的不是属于对象的

int main()
{
    A b;
    A a;
    cout << a.i;
    a.f();
    b.f();
    return 0;
}
