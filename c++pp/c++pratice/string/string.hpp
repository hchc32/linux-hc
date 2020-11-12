//string类的书写规范
#ifndef __STRING__
#define __STRING__

#include<cstring>
#include<iostream>
using namespace std;

class String
{

public:
    String(const char* cstr);
    String(const String& str);
    String& operator=(const String& str);
    char* get_c_str() const { return m_data; }
    ~String();

private:
    char* m_data;

};

inline
String::String(const char* cstr = 0)
{
    if(cstr)
    {
        m_data = new char[strlen(cstr)+1];
        strcpy(m_data , cstr);
    }
    else
    {
        m_data = new char[1];
        *m_data = '\0';
    }
}

inline
String::~String()
{
    delete[] m_data;
}

//拷贝构造函数
inline
String::String(const String& str)
{
    m_data = new char[strlen(str.m_data)+1];
    strcpy(m_data,str.m_data);
}

//拷贝赋值函数
inline String&
String::operator=(const String& str)
{
    //自我赋值
    if(this == &str)
    {
        return *this;
    }
    //m_data = this->m_data;
    delete[] m_data;
    m_data = new char[strlen(str.m_data)+1];
    strcpy(m_data,str.m_data);
    return *this;
}



ostream& operator<<(ostream& os , const String& str)
{
    os << str.get_c_str();
    return os;
}




#endif
