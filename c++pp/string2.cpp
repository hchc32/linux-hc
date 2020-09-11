#include <iostream>
#include<string>

using namespace std;

int main()
{
    string s("Hello World!!!");

    //转换成大写
    for(auto c : s)
    {
        c = toupper(c);
    }
    cout << s << endl;
    return 0;
}
