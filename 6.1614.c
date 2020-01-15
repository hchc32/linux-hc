#include <stdio.h>

int main()
{
    double a[8],b[8];
    int i,j;
    double sum=0;
    for(i=0;i<8;i++)
    {
        scanf("%lf",&a[i]);
        sum=sum+a[i];
        b[i]=sum;
    }
    for(i=0;i<8;i++)
    {
        printf("%7.2lf ",a[i]);
    }
    printf("\n");
    for(i=0;i<8;i++)
    {
        printf(" %7.2lf ",b[i]);
    }
    return 0;
}

