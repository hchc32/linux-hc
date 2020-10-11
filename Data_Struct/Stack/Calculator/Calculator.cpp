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
        cout << "表达式输入正确!" <<endl;
    }
    else
    {
        cout << "请输入正确的表达式:" << endl;
        return 0;
    }
    //中缀转后缀表达式
    exp.toPostfix();
    //后缀表达式求值
    cout << "res = " << exp.GetValue();
    return 0;
}
