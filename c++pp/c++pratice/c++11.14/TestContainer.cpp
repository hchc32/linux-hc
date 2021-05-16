#include <iostream>
#include <typeinfo>
#include "MyString.h"

using namespace std;

template<typename T>
void output_static_data(const T& myStr)
{
    cout <<typeid(myStr).name() << "--" <<endl;
    cout << "CCtor = " << T::CCtor
        <<  "MCtor = " << T::MCtor
        <<  "CAsgn = " << T::CAsgn
        <<  "MAsgn = " << T::MAsgn
        <<  "Dtor = " << T::Dtor
        <<  "Ctor = " << T::Ctor
        <<  "Dctor = " << T::DCtor
        << endl;
}


template<typename M,typename NM>
void test_moveable(M c1,NM c2,long& value)
{
    char buf[10];
    //测试moveable
    typedef typename iterator_traits<typename M::iterator>::value_type Vltype;
    clock_t timeStart = clock();
    for(long i = 0; i < value ; i++)
    {
        snprintf(buf,10,"%d",rand());
        auto ite = c1.end();
        //插入临时对象
        c1.insert(ite,Vltype(buf));
    }
    cout << "construction,milli-seconds: " << (clock()-timeStart) << endl;
    cout << "size() = " <<c1.size() << endl;
    output_static_data(*(c1.begin()));

    M c11(c1);
    //确保接下来不会用到c1
    M c12(std::move(c1));
    c11.swap(c12);
}

int main()
{
    return 0;
}

