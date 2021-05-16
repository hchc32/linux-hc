//unordered容器,关于hash function
#include <iostream>
using namespace std;
int main()
{
    cout << hash<char*>()("kiwi")<<endl;
    cout << hash<string>()("kiwi")<<endl;
    cout << hash<int>()(123) << endl;
    cout << hash<long>()(123L) << endl;
    cout << hash<string>()("Ace") << endl;
    cout << hash<const char*>()("Ace") << endl;
    cout << hash<char>()('A') << endl;
    cout << hash<float>()(3.1415926) << endl;
    return 0;
}

