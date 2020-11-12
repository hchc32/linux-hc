#include<bits/stdc++.h>
#define MAXN 9999
using namespace std;
string str1,str2;//str1为总的字符串   str2为匹配的字符串

int next1[10];//next1[i]代表str2字符串i位置之前的最长前缀

void getnext1array()//这个函数是求next1数组的
{
    if(str2.size()==1)//匹配的字符串只有一个字符
    {
        next1[0]=-1;
        next1[1]=0;
        return ;
    }
    next1[0]=-1;
    next1[1]=0;
    int i=2;
    int cn=0;
    cout << "str.size = " << str2.size() <<endl;
    while (i<=str2.size())
    {
        if(str2[i-1]==str2[cn])
        {
            //匹配到的前缀字符串相同个数
            cn++;
            next1[i]=cn;
            i++;
        }
        else if(cn>0)//这个条件满足，说明可以往前跳，让cn往前跳
            cn=next1[cn];
        else
            next1[i++]=0;//str2字符串i位置前面的字符串没有前缀

    }
    
}

int kmp()//kmp算法，如果匹配成功，返回1，否则，返回0
{
    int s1=0,s2=0;
    while (s1<str1.size()&&s2<str2.size())//两个指针都没有到达最后一个字符时，执行下面过程
    {
        if(str1[s1]==str2[s2])//str1字符串s1位置上的字符和str2字符串s2位置上的字符相等
        {
            s1++;
            s2++;
        }
        else if(next1[s2]==-1)
            s1++;
        else
            s2=next1[s2];
    }
    if(s2==str2.size())
        return 1;
    else
        return 0;
}
int main()
{
    cin>>str1>>str2;
    getnext1array();
    for(auto& it : next1)
    {
        cout << it << " ";
    }
    if(kmp())
        cout<<"匹配成功"<<endl;
    else
        cout<<"匹配失败"<<endl;
}
 
