#include <iostream>
#include <cstdlib>
using namespace std;

struct Complex
{
    int real,imag;
    //不可以隐士转换
    //explicit
    Complex(int re,int im=0):real(re),imag(im)
    {  }
    //nonexplicit one argument ctor可以隐士转换

    Complex operator+(const Complex& x)
    {
        return Complex((real+x.real),(imag+x.imag));
    }

    friend ostream& operator<<(ostream& os ,const Complex& x)
    {
        os << "real:" << x.real << " imag:" << x.imag;
        return os;
    }
};


int main()
{
    Complex c1(12,5);
    //explicit阻止隐士转换
    Complex c2 = c1+5;
    cout << c1 << endl;
    cout << c2 << endl;
    return 0;
}

