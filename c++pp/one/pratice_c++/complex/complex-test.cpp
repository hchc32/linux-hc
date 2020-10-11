#include <iostream>
#include"complex.hpp"

using namespace std;

int main()
{
    complex c1(2,1);
    complex c2(4,0);

    cout << c1 << endl;
    cout << c2 << endl;
    cout << c1+c2 << endl;
    cout << c1-c2 << endl;
    cout << c1+9 << endl;
    cout << 6+c1 << endl;
    cout << c1-8 << endl;
    cout << 9-c1 << endl;
    c1*=c2;

    cout << c1 <<endl;
    return 0;
}
