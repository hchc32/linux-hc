#include <stdio.h>
int jz(char *p);
int main()
{
    char s[100];
    fgets(s,2,stdin);
    printf("%d\n",jz(s));
    return 0;
}
int jz(char *p)
{
    int num=0;
    int i=0;
    while(p[i]!='\0')
    {
        num=num+p[i]*2;
        i++;
    }
    return num;
}

