#include <stdio.h>
int judge(char ch);
int main()
{
    char ch;
    int n;
    while((ch=getchar())!=EOF)
    {
        n=judge(ch);
        printf("%d\n",n);
    }
    return 0;
}

int judge(char ch)
{
    if(ch>='a'&&ch<='z')
        return (ch-'a'+1);
    else if(ch>='A'&&ch<='Z')
        return (ch-'A'+1);
    else
        return -1;
}
