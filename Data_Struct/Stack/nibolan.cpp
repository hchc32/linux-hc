#include<iostream>
#include<stack>
#include<stdlib.h>

#define MAX 1000

using namespace std;

char str[MAX];
void ReadInfexExp(char *str)
{
        
}
int main()
{
    stack<int> fuhao;
    stack<char> shuzi; 
    printf("请输入逆波兰表达式,输入#结束:");
    char c;
    scanf("%c",&c);
    while(c != '#')
    {
        if(c >= '0' && c <= '9')
        {
            int temp = atoi("c");
            shuzi.push(temp);
        }
        else
        {
            fuhao.push(c);
        }
    }

    switch()
    {

    }


    return 0;
}

