#include <stdio.h>
int main()
{
    double n1,n2;
    int i,n;
    while(1)
    {
        double sum1=0.0,sum2=0.0;
        int k=1;
        scanf("%d",&n);
        if(n<=0)
            return 0;
        for(i=1;i<=n;i++)
        {
            sum1=sum1+1.0/i*1.0;
        }
        for(i=1;i<=n;i++)
        {
            sum2=sum2+1.0/(i*k*1.0);
            k=k*-1;
        }
        printf("%.2lf  %.2lf\n",sum1,sum2);
    }
    return 0;
}
