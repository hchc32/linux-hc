#include <iostream>
#include <typeinfo>
#include <iostream>
#include <cxxabi.h>
using namespace std;
 
//成员模板
template <typename T>
class Array
{
public:
        template <typename T1>
        void output(T thistype, T1 othertype);
};
 
//成员模板
template <typename T>
template <typename T1>
void Array<T>::output(T thistype, T1 othertype)
{
        cout<<"thistype is "<<(typeid(thistype).name())<<", othertype is "<<typeid(othertype).name()<<endl;
}
 
int main(int argc, char*argv[])
{
        Array<int> arr;
        arr.output(10, "hello world");
        arr.output(20, 8.8);
        arr.output(30, false);
 
        return 0;
}
