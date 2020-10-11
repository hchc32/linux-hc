#include <stdio.h>
#include<string.h>
#define MAXLEN 10000
typedef struct
{
    char ch[MAXLEN];
    int len;             //字符串的长度
}String;
int next[255];

void get_next(String T , int* next)
{
    int i,j;
    i = 1; j = 0;
    next[1] = 0;
    while( i < T.len )
    {
        if(j == 0 || T.ch[i] == T.ch[j])
        {
            i++ ; j++ ;
            next[i] = j;
        }
        else
        {
            j = next[j];
        }
    }
}

int Index_Kmp(String S,String T,int pos)
{

    int i = pos;
    int j = 1;
    int next[255];
    get_next(T,next);
    while(i <= T.len && j<=T.len)
    {
        if( j == 0 || S.ch[i] == T.ch[j])
        {
            i++; j++;
        }
        else
        {
            j = next[i];
        }
    }
    if(j > T.ch[0])
    {
        return i - T.len;
    }
    else
    {
        return 0;
    }
}

int main()
{

    String T;
    String S;
    strcpy(S.ch,"hellolinux");
    scanf("%s",T.ch);
    get_next(T,next);
    printf(" %d ",next[0]);
    printf(" %d ",next[1]);
    printf(" %d ",next[2]);
    for(int i =0 ; next[i] != '\0'; i++)
    {
        printf(" %d ",next[i]);
    }

    printf("%d",Index_Kmp(S,T,0));
    return 0;
}

