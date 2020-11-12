#include <stdio.h>
#include<string.h>

int next[10];
/*
 kmp匹配
 返回值：
  1表示匹配成功
  0表示匹配失败
*/
//五号
int Index_KMP(char s[],char t[]);
void get_next(char t[],int next[]);

//aaaababab
//baba
int Index_KMP(char s[],char t[])
{
    int i = 0, j = 0;
    int slen = strlen(s);
    int tlen = strlen(t);
    while(i < slen && j < tlen )
    {
        if(s[i] == t[j])
        {
            i++;
            j++;
            continue;
        }
        else if(next[j] == -1)
        {
            i++;
        }
        else
        {
            j = next[j];
        }
    }
    if(j == tlen)
    {
        return 1;
    }
    return 0;
}

void get_next(char t[],int next[])
{
    int pos = 0;
    int tszie = strlen(t);
    if(tszie == 1)
    {
        next[0] = -1;
        next[1] = 0;
        return;
    }
    next[0] = -1;
    next[1] = 0;
    for(int i = 2; i <= tszie ;)
    {
        if(t[i-1] == t[pos])
        {
            pos++;
            next[i] = pos;
            i++;
        }
        else if(pos > 0)
        {
            pos = next[pos];
        }
        else
        {
            next[i] = 0;
            i++;
        }
    }
    /*
    for(int i = 0;i < 10; i++)
    {
        printf("%d ",next[i]);
    }
    */
}
int main()
{
    char str[10];
    char s[20];
    scanf("%s",s);
    scanf("%s",str);
    get_next(str,next);
    int ans = Index_KMP(s,str);
    if(ans == 1)
    {
        printf("yes");
    }
    else
    {
        printf("no!");
    }
    return 0;
}
