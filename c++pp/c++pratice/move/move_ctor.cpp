#include <iostream>
#include<vector>
#include<cstring>
using namespace std;
class Str
{
public:
    char* str;
    Str(char value[])
    {
        cout << "普通构造函数..." << endl;
        str = nullptr;
        int len = strlen(value);
        str = new char[len+1];       
        strcpy(str,value);
    }
    Str(const Str& s)
    {
        cout << "拷贝构造函数..." << endl;
        str = nullptr;
        int len = strlen(s.str);
        str = new char[len+1];
        strcpy(str,s.str);
    }
    Str(Str &&s)
    {
        cout << "移动构造函数..." << endl;
        str = nullptr;
        str = s.str;
        s.str = nullptr;
    }
    ~Str()
    {
        cout << "析构函数..." << endl;
        if(str != nullptr)
        {
            delete []str;
            str = nullptr;
        }
    }
};

int main()
{
    char value[] = "l love linux";
    Str s(value);
    vector<Str> vs;
    //ｓ当前为左值
    vs.push_back(std::move(s));
    cout << vs[0].str << endl;
    if(s.str != nullptr)
    {
        cout << s.str << endl;
    }
    return 0;
}

