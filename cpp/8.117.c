#include <stdio.h>                                                                                               
#define HEAD 0.15
#define MID 0.2
#define LAST 0.25

int main()
{
    int time;
    char select='\0';
    double extra=0;
    double sum;
    double mon;

    while(select==0)
    {
    printf("******************************************************************");
    printf("Enter the number corresponding to the desired pay rate or action: ");
    printf("a $8.75/hr         b $9.33/hr");
    printf("c $10.00/hr         d $11.20/hr");
    printf("q quit");
    printf("*******************************************************************");
    scanf("%c",&select);
    switch(select)
    {
        case 'a':mon=8.75;break;
        case 'b':mon=9.33;break;
        case 'c':mon=10.00;break;
        case 'd':mon=11.20;break;
        case 'q':return 0;
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
