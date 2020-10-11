#include <stdio.h>
#include<string.h>
#define MAXLEN 50

typedef struct
{
    char ch[MAXLEN];
    int len;             //字符串的长度
}String;

int String_BF(String S , int pos ,String T)
{
    int i = pos;
    int j = 0;
    while(i < S.len && j < T.len)
    {
        if(S.ch[i] == T.ch[j])
        {
            i++; j++;
        }
        else
        {
            i = i-j+1; 
            j = 0;
        }
    }
    if(j >= T.len) return i-T.len;  
    else return 0;
}

int main()
{
    String temp1;
    String temp2;
    strcpy(temp1.ch,"Hellolinux");
    temp1.len = strlen("Hellolinux");
    strcpy(temp2.ch,"olinu");
    temp2.len = strlen("olinu");
    printf("%d\n",String_BF(temp1,0,temp2));
    return 0;
}

