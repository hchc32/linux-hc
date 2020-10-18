//类似于
#include <iostream>
#include<string>

using namespace std;

int main()
{
    string str;
    cin >> str;
   // whi0le( 
    auto a = str.begin();
    //尾部迭带器,指向的是最后一个元素的下一个位置
    auto b = str.end()-1;
   // {
   cout <<  *a << endl;
   cout <<  *b << endl;
    //}

    return 0;
}
