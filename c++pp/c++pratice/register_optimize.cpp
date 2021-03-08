/*
   c++寄存器优化:
        由于我们告诉编译器data 是一个const常量,所以编译器会将data放入寄存器里，加快下一次的读取速度.
        然后再对ｐ去常，通过变量ｑ修改data再内存中的值．
        打印结果的时候，读取的是寄存器的值，而不是内存中的值．
 */
#include"register_optimize.h"

int main()
{
    hc1::display();
    hc::display();
    return 0;
}

