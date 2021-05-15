#include <iostream>
using namespace std;

//type alias
typedef void (*func)(int,int);

//alias template
template<typename T>
struct Container
{
    using value_type=T;
};

template<class CharT> using mystring = \
            std::basic_string<CharT,std::char_traits<CharT>>;

int main()
{
    func a;
    return 0;
}

