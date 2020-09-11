#include <iostream>
#include<string>

using namespace std;

int main()
{
    string str = "Hello Linux ,,,!!!!";
    
    //确保count的类型和字符串size的类型一样
    //string::size_type
    decltype(str.size()) count = 0;

    for(auto c: str )
    {
        if(ispunct(c))
        {
            count++;
        }
    }
    cout << "count = " << count <<  endl;
    return 0;
}
