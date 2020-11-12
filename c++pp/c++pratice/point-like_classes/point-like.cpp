#include <iostream>

using namespace std;

template<typename T>
class my_share_ptr
{
public:
    //指针的必须的操作符重载
    T& operator*() const { return *px; }
    T* operator->() const { return px; }

    my_share_ptr(T* p):px(p) {}
private:
    T* px;
    long* pn;
};


