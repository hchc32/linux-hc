#include <iostream>
#include <exception>
using namespace std;
//void foo()noexcept(true);
void foo()noexcept;

void on_terminate()
{
    cout << "terninate function called!" << endl;
    cin.get();
}

int main()
{
    set_terminate(on_terminate);
    throw exception();
    cout << "terminate function not called!" << endl;
    std::cout << "Hello world" << std::endl;
    return 0;
}

