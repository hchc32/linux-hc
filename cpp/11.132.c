#include <stdio.h>
#include<string.h>
void fun(char *chstr,int n);
int main()
{
    char chstr[100];
    int i,n;
    scanf("%d",&n);
    fgets (chstr,100,stdin);
    fun(chstr,n);
    for(i=0;i<strlen(chstr);i++)
        printf("%c",chstr[i]);
    return 0;
}
void fun(char *chstr,int n)
{
    char ch;
    int i=0;
    ch=getchar();
    while(ch!=EOF && ch!=' ' && ch!='\n' && ch!='\t' && i+1!=n)
    {
        chstr[i]=ch;
        i++;
        ch=getchar();
    }
    chstr[i]='\0';
}

