#include <iostream>
using namespace std;

void swap(int& x,int& y);

int main()
{
    auto a{5},b{10};
    cout << "Before: a=" << a << " b=" << b <<endl;
    swap(a,b);
    cout << "After: a=" << a << " b=" << b <<endl;
    return 0;
}


void  swap(int& x,int& y)
{
    int t;
    t = x;
    x = y;
    y = t;
}

