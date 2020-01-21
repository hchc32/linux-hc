#include <stdio.h>
#include<string.h>
void fun(char *chstr);                               
int main()
{
    char chstr[100];
    int i;
    fun(chstr);
    for(i=0;i<strlen(chstr);i++)
        printf("%c",chstr[i]);
    return 0;
}
void fun(char *chstr)
{
    char ch;
    int i=0;
    while((ch=getchar())!=EOF)
    {
        chstr[i]=ch;
        i++;
    }
    chstr[i]='\0';
}

