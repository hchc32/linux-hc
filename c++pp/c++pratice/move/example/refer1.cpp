#include <iostream>

using namespace std;

//copy
/*
template<typename fun,typename a,typename b>
void exec(fun f,a tmpa,b tmpb)
{
    f(tmpa,tmpb);
}
*/
void tmp(int a,int b)
{
    cout << a*b << endl;
}
template<typename fun,typename a,typename b>
void exec(fun f,a &tmpa,b &tmpb)
{
    f(tmpa,tmpb);
}


int main()
{
    int a = 3;
    int b = 4;
    exec(tmp,a,6);
    return 0;
}

