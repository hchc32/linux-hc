#include <iostream>

using namespace std;

const double pi = 3.14;

class Circle
{
public:
    double fun(int R) { return pi*R*2; }
    int R;
};
int main()
{
    Circle a;
    cin >> a.R; 
    cout << a.fun(a.R);
    return 0;
}
