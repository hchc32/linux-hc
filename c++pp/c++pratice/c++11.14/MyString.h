#pragma once
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<string>
#include<unistd.h>
using namespace std;
class MyString
{
public:
    //各种函数调用的次数
    static size_t DCtor; 
    static size_t Ctor;
    static size_t CCtor;
    static size_t CAsgn;
    static size_t MCtor;
    static size_t MAsgn;
    static size_t Dtor;
public:
    //DCtor
    MyString():_data(nullptr),_len(0) { ++DCtor; }

    //Ctor
    MyString(const char *p):_len(strlen(p))
    {
        ++Ctor;
        _init_data(p);
    }

    //CCtor
    MyString(const MyString& str):_len(str._len)
    {
        ++CCtor;
        _init_data(str._data);
    }

    //MCtor
    MyString(MyString&& str)noexcept
        :_data(str._data),_len(str._len)
        {
            ++MCtor;
            str._len=0;
            str._data=nullptr;
        }

    //CAsgn
    MyString& operator=(const MyString& str)
    {
        ++CAsgn;
        //自我赋值检查
        if(this != &str)
        {
            //删除原有数据
            if(_data) delete _data;
            _len=str._len;
            _init_data(str._data);
        }
        return *this;
    }

    //MAsgn
    MyString& operator=(MyString&& str)noexcept
    {
        ++MAsgn;
        if(this != &str)
        {
            if(_data) delete _data;
            _len = str._len;
            _data = str._data;
            str._len = 0;
            str._data = nullptr;
        }
        return *this;
    }

    //Dtor
    virtual ~MyString() { ++Dtor; if(_data) delete _data; }

public:
    //为了set的测试
    bool operator<(const MyString&rhs) const
    {
        return string(this->_data) < string(rhs._data);
    }

    //为了set的测试
    bool operator==(const MyString&rhs) const
    {
        return string(this->_data) == string(rhs._data);
    }

    char* get() const { return _data; }

private:
    char *_data;
    size_t _len;
    void _init_data(const char *s)
    {
        _data = new char[_len+1];
        memcpy(_data,s,_len);
        _data[_len] = '\0';
    }
};
size_t MyString::DCtor=0;
size_t MyString::Ctor=0;
size_t MyString::CCtor=0;
size_t MyString::CAsgn=0;
size_t MyString::MCtor=0;
size_t MyString::MAsgn=0;
size_t MyString::Dtor=0;

