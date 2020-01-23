#include <stdio.h>
#define TIME 60
int main()
{
    int min;
    int hour;
    while(1)
    {
        scanf("%d",&min);
        if(min==0)
            break;
        hour=min/TIME;
        min=min-hour*60;
        printf("时间为%02d:%02d\n",hour,min);
    }


    return 0;
}

