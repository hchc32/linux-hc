#include <iostream>

using namespace std;

int main()
{
    int a = 1;
    int i = 0;
    int &b = a;
    const int &c = i;
    
    std::cout << "a=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
    a = 2;
    i = 3;

    std::cout << "a=" << b << std::endl;
    std::cout << "c=" << c << std::endl;

    return 0;
}
