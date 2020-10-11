#include <iostream>

class complex
{
public:
    complex(int aa=0,int bb=0) : a(aa),b(bb) {}
    
    //complex() : a(0),b(0) {}
    //两个构造函数是冲突的
private:
    int a;
    int b;
};



int main()
{

    complex temp;

    return 0;
}
