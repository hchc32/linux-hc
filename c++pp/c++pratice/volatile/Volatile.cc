#include<iostream>

using namespace std;

int fn(int afn)
{
    return afn+1;
}

int main()
{
    volatile int a = 5;
    int b = 10;
    int c = 20;
    int d;
    cin >> c;

    a = fn(c);//1+1
    b = a + 1;//3
    d = fn(b);//4
    cout << a << b << c << d;
    return 0;
}