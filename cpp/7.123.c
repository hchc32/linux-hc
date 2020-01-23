#include <stdio.h>

int main()
{
    int n,c1=0,c2=0;
    float sum1=0,sum2=0;
    do
    {
        scanf("%d",&n);
        if(n==0)
            break;
        if(n%2==0)
        {
            c1++;
            sum1+=n;
        }
        else
        {
            c2++;
            sum2+=n;
        }
    }
    while(1);
    printf("偶数的个数%d 偶数的平均值%.2f 奇数的个数%d 奇数的平均值%.2f",c1,sum1*1.00/c1,c2,sum2*1.00/c2);
    return 0;
}

