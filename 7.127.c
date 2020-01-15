#include <stdio.h>
#define HEAD 0.15
#define MID 0.2
#define LAST 0.25

int main()
{
    int time;
    double extra=0;
    double sum;
    scanf("%d",&time);
    if(time>40)
        sum=(40-time)*1.5*1000+40*1000;
    else
        sum=time*1000;
    if(sum>300)
        extra=extra+300*HEAD;
    if(sum<=450)
        extra=extra+150*MID;
    if(sum>450)
        extra=extra+(sum-450)*LAST;
    printf("%7.2lf\n",sum);
    printf("%7.2lf\n",extra);
    printf("%7.2lf\n",sum-extra);
    return 0;
}

