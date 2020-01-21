#include <stdio.h>
#include<ctype.h>

void fun(char *chstr);
int main()
{
    char chstr[1000];
    fgets(chstr,100,stdin);
    getchar();
    fun(chstr);
    return 0;
}

void fun(char *chstr)
{
    char ch;
    while((ch=getchar())!=EOF && isspace(ch=getchar()))
    {
        continue;
    }
    if(ch==EOF)
        return;
    while(!isspace(ch))
    {
        *chstr=ch;
        chstr++;
    }
    while(ch!='\n')
        ch=getchar();
}
