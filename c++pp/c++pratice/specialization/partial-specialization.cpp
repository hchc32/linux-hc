#include <iostream>
using namespace std;
//模板偏特化参数的个数特化
template<typename A,typename B>
struct test
{
    A a;
    B b;
};
template<typename B>
struct test<int,B>
{
    int a;
    B b;
};
//范围特化
template<typename T>
class C
{
public:
    T c;
};
template<typename T>
class C<T*>
{
public:
    T* c;
};
int main()
{
    C<int> c1;
    C<int*> c2;
    return 0;
}
