#include <iostream>
#include<string>

using namespace std;

int main()
{
    string line;
    while( cin >> line )
    {
        if(!line.empty())
        {
            cout << line.size() << endl;
            cout << line << endl;
            cout << "......." << endl;
        }
    }
    return 0;
}
