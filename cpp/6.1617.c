#include <stdio.h>

int main()
{
    int money=100;
    int year=0;
    while(money>0)
    {
        money=money+money*0.08;
        money=money-10;
        year++;
    }
    printf("%d年后钱会取完！",year);
    return 0;
}

