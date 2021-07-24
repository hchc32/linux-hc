#include <iostream>
#include<string>
using namespace std;

int main()
{
    int *dp = new int[101];
    string a, b;
    for (int i=0;i<5;i++)
    {
        cin >> dp[i];
    }
    for (int i=0;i<5;i++)
    {
        cout << dp[i] << endl;
    }
        //cin >> a >> b;
        cout << "The string is " << a << b << endl;
    return 0;
}
