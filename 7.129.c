#include <stdio.h>
#include<math.h>
int prime(int n);
int main()
{
    int i;
    int k;
    scanf("%d",&i);
    for(k=2;k<=i;k++)
    {
        if(prime(k))
            printf("%d\n",k);
    }

    return 0;
}
int prime(int n)
{
    int i;
    for(i=2;i<=sqrt(n);i++)
    {
        if(n%i==0)
            return 0;
    }
    return 1;
}
