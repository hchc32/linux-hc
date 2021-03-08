#ifndef _REGISTER_OPTIMIZE_H_
#define _REGISTER_OPTIMIZE_H_

#include <iostream>

using namespace std;
namespace hc1
{
    void display()
    {
        int const data = 100;
        int const * p = &data;
        //const_cast<type>(express) 去常量属性
        int * const q = const_cast<int*>(p);
        *q = 200;
        cout << data << " " << *p << " " << *q << endl;
        cout << &data << endl;
        cout << p << endl;
        cout << q << endl;
    }
}

namespace hc
{
    void display()
    {
        volatile int const data = 100;
        volatile int const * p = &data;
        //const_cast<type>(express) 去常量属性
        int * const q = const_cast<int*>(p);
        *q = 200;
        cout << data << " " << *p << " " << *q << endl;
        cout << (void*)&data << endl;
        cout << (void*)p << endl;
        cout << q << endl;
    }
}
#endif
