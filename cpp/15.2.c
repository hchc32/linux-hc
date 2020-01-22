#include <stdio.h>
int count(int a);

int main()
{
    int a;
    while(scanf("%d",&a)!=EOF)
    {
        printf("%d 的位有%d个\n",a,count(a)); 
    }
    return 0;
}

int count(int a)
{
    int count=0;
    while(a)
    {
        count++;
        a>>=1;
    }
    return count;
}

