//Alias template + template template parameter
#include <iostream>
#include <vector>
#define SIZE 100000

template<typename T>
using Vec = std::vector<T,std::allocator<T>>;

//模板模板参数不可以深层次进行推导．
//vector有两个参数
//所以要进行Alias template
template<typename T,template<class>
        class Container
        >
class XCIs
{
private:
    Container<T> c;
public:
    XCIs()
    {
        for(long i = 0; i < SIZE; i++)
            c.insert(c.end(),T());
        Container<T> c1(c);
        Container<T> c2(std::move(c));
        c1.swap(c2);
    }
};


int main()
{
    XCIs<std::string,Vec> c1;
    return 0;
}

