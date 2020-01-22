#include <stdio.h>
int find_(int a,int n);
int main()
{
    int a;
    while(scanf("%d",&a)!=EOF)
    {
        find_(a,2);
        printf("%d\n",find_(a,2));
    }
    return 0;
}

int find_(int a,int n)
{
    int flag=0;
    if(a!=0)
        return(((a-1)>>1)&n);
    return a;
}

