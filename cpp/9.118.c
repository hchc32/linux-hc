#include <stdio.h>
double power(double n,int p);
int main(void)
{
    double x,xpow,spow;
    int exp;
    printf("Enter a number and the positive integer power");
    printf("to which\nthe number will be raised.Enter q");
    printf("to quit.\n");
    while(scanf("%lf%d",&x,&exp)==2)
    {
        spow=power(x,exp);
        printf("%.3g to the power %d is %.5g\n",x,exp,xpow);
        printf("Enter next pair of numbers or q to quit.\n");
    }
    printf("Hope you enioyed this power trip -- bye!\n");
    return 0;
}
double power(double n,int p)
{
    double pow=1;
    int i;
    if(p==0)
        return 1;
    if(n==0)
        return 0;
    if(n<=0)
    {
        for(i=1;i<=p;i++)
        {
            pow*=n;
            return 1/pow;
        }
    }
    for(i=1;i<=p;i++)
        pow*=n;
    return pow;
}
