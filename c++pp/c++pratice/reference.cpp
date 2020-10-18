#include <iostream>

using namespace std;

class A
{
public:
    int i;
    A():i(0) {}
};

A f()
{
    A a;
    return a;
}
int main()
{
    A b;
    b.i = 20;
    A c = f() = b ;
    cout << c.i << endl;
    return 0;
}
