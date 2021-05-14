//variadic template　数量不定的模板参数
#include <iostream>
#include <bitset>
#include <cstdlib>
using namespace std;

//相当于递归结束条件
void printX()
{

}

template<typename T,typename... Types>
void printX(const T& firstArg , const Types&... args)
{
    cout << firstArg <<endl;
    cout << sizeof(firstArg) << endl;
    printX(args...);
}

template<typename... Types>
void printX(const Types&... args)
{
    cout << "666" << endl;
}

int main()
{
    printX(7.5,"hello",bitset<16>(377),42);
    return 0;
}

