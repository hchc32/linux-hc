#include<iostream>
#include<algorithm>
#include"exp.hpp"
#include"stack.hpp"
using namespace std;


int main()
{
    string s; 
    cin >> s;
    Exp exp(s);

    //判断输入是否合法
    if(exp.IfLegal())
    {
        cout << "Legal!" <<endl;
    }
    else
    {
        cout << "Not Legal!" << endl;
    }
    exp.toPostfix();
    cout << "res = " << exp.GetValue();
    return 0;
}
