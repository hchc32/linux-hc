#include <iostream>

using namespace std;


int main()
{
    string str = "hellolinux";
    string &msg = str;
    cout << msg << endl;

    return 0;
}
