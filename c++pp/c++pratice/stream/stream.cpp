#include <iostream>
using namespace std;
int main()
{
    int input;
    int sum = 0;
    while(cin >> input)
    {
        sum+=input;
    }
    cout << "sum = " << sum << endl;
    //数据没有读完，但是遇到非法字符
    if(cin.fail() && !cin.eof())
    {
        cout << "updateing.." << endl;
        cin.clear();
        while(putchar(cin.get()) != '\n')
        {
            continue;
        }
    }
    else
    {
        cout << "eofbit == 1" << endl;
        exit(1);
    }
    return 0;
}

