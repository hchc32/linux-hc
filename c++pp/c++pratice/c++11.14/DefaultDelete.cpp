//default,只能用于Big-Five
//delete,可以用于任意函数身上.
#include <iostream>
using namespace std;

class Zoo
{
public:
    //default需要编译器提供的默认的
    Zoo()=default;
    Zoo(int i1,int i2):d1(i1),d2(i2) {  }
    //delete不需要这个函数
    Zoo(const Zoo&)=delete;
    Zoo(Zoo&&)=default;
    Zoo& operator=(const Zoo&)=default;
    Zoo& operator=(const Zoo&&)=delete;
    //!Error virtual ~Zoo()=0;
    virtual ~Zoo() {  }
    friend ostream& operator<<(ostream& os,const Zoo& a)
    {
        return os << a.d1 << " " << a.d2 << endl;
    }
private:
    int d1,d2;
};


int main()
{
    Zoo a(1,2);
    Zoo b;
    b=a;
    cout << a;
    cout << b;
    return 0;
}

