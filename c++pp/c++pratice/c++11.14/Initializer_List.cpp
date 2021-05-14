//Initializer_list 的包可以被拆解,但只限于固定数量的参数
#include <iostream>
using namespace std;
void print(initializer_list<int> vals)
{
    for(auto p = vals.begin(); p!=vals.end();++p)
    {
        cout << *p << endl;
    }
}

class P
{
public:
    P(int a, int b)
    {
        cout << "P(int,int) = " << a << ' ' <<  b << endl;
    }
    P(initializer_list<int> initlist)
    {
        cout  << "P(initializer_list) = ";
        for(auto i : initlist)
        {
            cout << i << ' ';
        }
        cout << endl;
    }
};

int main()
{
    //test_print
    //print({12,3,5,6,7,6,5,4});
    
    //test P
    P p(77,5);
    P q{77,5};
    P r{1,2,3};
    return 0;
}

