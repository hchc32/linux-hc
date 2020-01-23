#include <stdio.h>
#include<math.h>
int main()
{
    int a[8];
    int i,j;
    for(i=0;i<=8;i++)
    {
        a[i]=pow(2,i);
    }
    i=0;
    do
    {
        printf("%d ",a[i]);
        i++;
    }
    while(i<=8);
    return 0;
}

