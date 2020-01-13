#include <stdio.h>

int main()
{
    int day,week,days;
    while(1)
    {
        scanf("%d",&day);
        if(day<=0)
            break;
        else
        {
            week=day/7;
            days=day-week*7;
            printf("%d days are %d weeks, %d days\n",day,week,days);
        }
    }
    return 0;
}

