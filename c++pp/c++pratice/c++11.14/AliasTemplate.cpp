#include <iostream>
#include <vector>
using namespace std;
#define SIZE 1000000000
/*AliasTemplate
template<typename T>
using Vec = std::vector<T,std::allocator<T>>;
*/
template<typename T>
void output_static_data(T& a)
{
    cout << a << endl;
}

template<typename Container>
void test_moveable(Container c)
{
    typedef typename iterator_traits<typename Container::iterator>::value_type Valtype;
    for(long i = 0; i < SIZE; i++)
    {
        c.insert(c.end(),Valtype());
    }
    output_static_data(*(c.begin()));
    Container c1(c);
    Container c2(std::move(c));
    c1.swap(c2);
}

int main()
{
    test_moveable(vector<string>());
    return 0;
}

