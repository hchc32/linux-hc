#include <iostream>
#include"string.hpp"

int main()
{
    String s1;
    cout << s1 << endl;
    String s2(s1);
    cout << s2 << endl;
    String s3("Hello Linux!");
    cout << s3 << endl;
    String s4 = s3;
    cout << s4 << endl;
    return 0;
}
