//数量不定的模板参数
#include <iostream>
#include<cstdlib>


void print()
{
    std::cout<<std::endl;
}

template<typename T,typename... Types>
void print(const T& firstArg , const Types&... args)
{
    std::cout<< firstArg << "left:"<<sizeof...(args)<<std::endl;
    print(args...);
}

int main()
{
    print(1,2,'a',1.0);
    return 0;
}
