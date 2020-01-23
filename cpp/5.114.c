#include <stdio.h>

int main()
{
    int length;
    while(1)
    {
        printf("Enter a height in centimeters:");
        scanf("%d",&length);
        if(length<=0)
            break;
        else
            printf("%.1d cm = %.1lf inches\n",length,length/2.54);
    }

    return 0;
}

