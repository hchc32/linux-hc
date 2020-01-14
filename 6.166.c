#include <stdio.h>

int main()
{
    int i;
    int j,n1,n2;
    scanf("%d %d",&n1,&n2);
    for(i=n1;i<=n2;i++)
    {
        printf("%d\t%d\t%d\n",i,i*i,i*i*i);
    }
    return 0;
}

