#include <iostream>
#include<string.h>
#include<unistd.h>

using namespace std;

class A
{
public:
    A(int aa) { a = aa; }
    ~A() { cout << "Bye A()::~A a = "  << a << endl; } 
private:
      int a;
};
int main()
{
    {
        {
            A a = A(12);
            A b = A(10);
            sleep(1);

        }
        //A b;
    }
    sleep(1);
    return 0;
}
