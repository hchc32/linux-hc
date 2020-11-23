#include <iostream>
using namespace std;

class X {  };
class Y:public virtual X {  };
class Z:public virtual X {  };
class A:public Y,public Z {  };
int main()
{
    X a,b;
    //1
    //空class
    //a与b的地址不一样
    cout << sizeof(X) <<" ";
    cout << sizeof(a) <<" ";
    cout << sizeof(b) <<" ";
    if(&a == &b)
    {
        cout << "&a==&b" << endl;
    }
    else
    {
        cout << "&a!=&b" << endl;
    }
    Y c,d;
    //8
    cout << sizeof(Y) << " ";
    cout << sizeof(c) << " ";
    cout << sizeof(d) << " ";
    if(&c == &d)
    {
        cout << "&c==&d" << endl;
    }
    else
    {
        cout << "&c!=&d" << endl;
    }

    //8
    cout << sizeof(Z) <<" ";
    //16
    cout << sizeof(A) <<" ";
    return 0;
}
