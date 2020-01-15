#include <stdio.h>

int main()
{
    int rab=5;
    int week=0;
    while(rab<=150)
    {
        rab=rab-1;
        rab=2*rab;
        week++;
        printf("第%d周博士的朋友数量%d个!\n",week,rab);
    }
    printf("%d周后超过邓巴数",week);

    return 0;
}

