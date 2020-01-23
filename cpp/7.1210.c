#include <stdio.h>
#define HEAD 0.15
#define LAST 0.28
int main()
{
    int select;
    double mon,sum;
    while(1)
    {
        printf("*******************************\n");
        printf("Please choose your tax type.\n");
        printf("1. single\n2. householder\n");
        printf("3. married,common\n4. married,divorced\n");
        printf("5. quit\n\n");
        scanf("%d",&select);
         switch (select)
            {
            case 1: mon = 17850; break;
            case 2: mon = 23900; break;
            case 3: mon = 29750; break;
            case 4: mon = 14875; break;
            case 5: break;
            default: 
                printf("\nPlease enter right number.\n"); 
                break;
            }
    scanf("%lf",&sum);
    double extra=0;
    if(sum>mon)
    {
        extra=mon*HEAD;
        extra=extra+(sum-mon)*LAST;
    }
    printf("%lf",extra);
    }
    return 0;
}

