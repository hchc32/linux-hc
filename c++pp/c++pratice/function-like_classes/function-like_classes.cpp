#include <iostream>

using namespace  std;
namespace test1
{
    void echo()
    {
        std::cout << "test1 rcho" <<std::endl;
    }
}

namespace test2
{
    void echo()
    {
        std::cout << "test2 rcho" <<std::endl;
    }
}

void echo()
{
    std::cout << "main rcho" <<std::endl;
}
/*
struct unary
{
    typedef int arguement_type;
    typedef char ar;
    typedef char a;
};
*/

int main()
{
    test1::echo();
    test2::echo();
    ::echo();
    return 0;
}
