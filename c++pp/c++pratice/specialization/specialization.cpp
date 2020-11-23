#include <iostream>
using namespace std;
//模板泛化
template<class T>
struct my_hash
{
    T operator() (T x) const {
    std::cout << "特化" << sizeof(T)<<std::endl;

        return x;}
};
template<>
struct my_hash<char>
{
    char operator() (char x) const {return x;}
};
template<>
struct my_hash<long>
{
    long operator() (long x) const {return x;}
};
template<>
struct my_hash<int>
{
    int operator() (int x) const {return x;}
};
int main()
{
    cout << my_hash<long>() (1000) <<endl;
    cout << my_hash<char>() ('a') <<endl;
    cout << my_hash<int>() (10) <<endl;
    cout << my_hash<double>() (1.1) <<endl;
    return 0;
}
