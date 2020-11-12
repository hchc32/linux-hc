//成员模板
#include <iostream>

using namespace std;

template<class T1, class T2>
struct pair
{
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    //无参构造函数
    pair():first(T1()) , second(T2()) {}

    //有参构造函数
    pair(cont T1 &a , const T2 &b):first(a) , second(b) {}
};



int main()
{

    return 0;
}
