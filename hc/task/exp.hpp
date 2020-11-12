#ifndef _EXP_H_
#define _EXP_H_

#include<iostream>
#include<cstdlib>
#include<vector>
#include<string>
#include<ctype.h>
#include<cmath>
#include"stack.hpp"

using namespace std;

class Exp
{
public:
    Exp(string s) 
    {
        //s=s;
        this->s = s;
    }
    ~Exp() {}
    bool IfLegal();
    void toPostfix();
    int GetPriority(char c);
    double GetValue();
private:
    string s;
    //保存后缀表达式
    vector <char> v;
};



double Exp::GetValue()
{
    int size = v.size();
    stack<double> st;
    for(int i = 0 ; i < size ; i++)
    {
        //是数字
        if(isalnum(v[i]))
        {
            //对小数进行处理
            int flag = -1; 
            double res = v[i] - 48;
            while(i + 1 < size && isalnum(v[i+1]))
            {
                res *= 10;
                res += (v[++i]-48);
            }
            if(i + 1 < size && v[i+1] == '.')
            {
                i++;
                while(i + 1 < size && isalnum(v[i+1]))
                {
                    res += (v[++i] - 48)*pow(10,flag--);
                }
            }

            //结果进行压栈
            st.push(res);
            
            i++;

        }
        //是符号
        else
        {
            if(v[i] == '+')
            {
                double temp1 = st.top();
                st.pop();
                double temp2 = st.top();
                st.pop();
                double res = temp2 + temp1;
                st.push(res);
            }
            else if(v[i] == '-')
            {
                double temp1 = st.top();
                st.pop();
                double temp2 = st.top();
                st.pop();
                double res = temp2 - temp1;
                st.push(res);
            }
            else if(v[i] == '*')
            {
                double temp1 = st.top();
                st.pop();
                double temp2 = st.top();
                st.pop();
                double res = temp2 * temp1;
                st.push(res);
            }
            else if(v[i] == '/')
            {
                double temp1 = st.top();
                st.pop();
                double temp2 = st.top();
                st.pop();
                double res = temp2 / temp1;
                st.push(res);
            }
        }
        //是小数点
        //是#
    }
    return st.top();

}

bool Exp::IfLegal()
{
    int size = s.size();
    if(s[0] == '/' || s[0] == '*')
    {
        return false;
    }
    if(s[size-1] == '/' || s[size-1] == '*')
    {
        return false;
    }
    int count = 0;
    for(int i = 0; i < size ; i++)
    {
        //()中没有运算的对象
        if(s[i] == '(')
        {
            if(i+1 < size && s[i+1] == ')')
            {
                return false;
            }
            count++;
        }
        else if(s[i] == ')')
        {
            count--;
        }
        else if(s[i] == '/')
        {
            if(i+1 < size && s[i+1] == '0' || \
               s[i+1] == '*')
            {
                return false;
            }
        }
        else if(s[i] == '*')
        {
            if(i+1 < size && s[i+1] == '/')
            {
                return false;    
            }
        }
        else if(s[i] == '+')
        {
            if(i+1 < size && s[i+1] == ')' || s[i+1] == '(')
            {
                return false;
            }
        }
    }
    if( count )
    {
        return false;
    }
    return true;
}

int Exp::GetPriority(char c)
{
    if(c == '(')    
        return 1;
    if(c == '+' || c == '-') 
        return 2;
    else  
        return 3;
}


void Exp::toPostfix()
{
    string tmp;
    stack<char> st;
    int size = s.size();

    //预处理符号
    for(int i = 0; i < size ; i++)
    {
        if(s[i] == '-')
        {
            if(i == 0 || (i-1>=0 && s[i-1] == '('))
            {
                s.insert(i,1,'0');
            }
        }
    }
    size = s.size();
    for(int i = 0; i < size ; i++)
    {
        //是字母或者数字
        if(isalnum(s[i]))
        {
            v.push_back(s[i]);
            //处理小数点和多位数
            while(i+1 < size && (isalnum(s[i+1]) || s[i+1] == '.'))
            {
                v.push_back(s[++i]);
            }
            //加一个分隔符号
            v.push_back('#');

        }
        else if(st.empty())
        {
             st.push(s[i]);
        }
        else
        {
            if(s[i] == '(') 
            {
                st.push(s[i]);
            }
            else if(s[i] == ')')
            {
                //将括号内的符号都进行出栈操作
                while(!st.empty() && st.top() != '(')
                {
                    v.push_back(st.top());
                    st.pop();
                }
                st.pop();
            }
            else
            {
                while(!st.empty() && GetPriority(st.top()) >= GetPriority(s[i]))
                {
                    v.push_back(st.top());
                    st.pop();
                }
                //操作符号入栈
                st.push(s[i]);
            }
        }
    }
    while(!st.empty())
    {
        v.push_back(st.top());
        st.pop();
    }
    for(int i = 0; i < (int)v.size() ; i++)
    {
        cout << v[i];
    }
    cout << endl;
    return;
}
#endif
