#include <stdio.h>
#include<ctype.h>

void fun(char *chstr,int n);
int main()
{
    char chstr[1000];
    fgets(chstr,100,stdin);
    int n;
    scanf("%d",&n);
    getchar();
    fun(chstr,n);
    return 0;
}

void fun(char *chstr,int n)
{
    char ch;
    int i;
    while((ch=getchar())!=EOF && isspace(ch=getchar()))
    {
        continue;
    }
    if(ch==EOF)
        return;
    while(!isspace(ch)&&i<=n)
    {
        i++;
        *chstr=ch;
        chstr++;
    }
    while(ch!='\n')
        ch=getchar();
}                                                                         

