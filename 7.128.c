#include <stdio.h>                               
#define HEAD 0.15
#define MID 0.2
#define LAST 0.25

int main()
{
    int time,select=0;
    double extra=0;
    double sum;
    double mon;

    while(select==0)
    {
    printf("******************************************************************");
    printf("Enter the number corresponding to the desired pay rate or action: ");
    printf("1 $8.75/hr         2 $9.33/hr");
    printf("3 $10.00/hr         4 $11.20/hr");
    printf("5 quit");
    printf("*******************************************************************");
    scanf("%d",&select);
    switch(select)
    {
        case 1:mon=8.75;break;
        case 2:mon=9.33;break;
        case 3:mon=10.00;break;
        case 4:mon=11.20;break;
        case 5:break;
        default:
               {
                   printf("请重新输入!");
                    continue;
               }
    }
    }
    scanf("%d",&time);
    
    if(time>40)
        sum=(40-time)*1.5*mon+40*mon;
    else
        sum=time*mon;
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

