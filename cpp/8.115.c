#include <stdio.h>

int main()
{
    int guess=50;
    int n=-1;
    while(n!=guess)
    {
        scanf("%d",&n);
        if(n>guess)
        {
            printf("猜的数字太大了！\n请继续:");
            continue;
        }
        if(n<guess)
        {
            printf("猜的数字太小了！\n请继续:");
            continue;
        }
    }
    printf("恭喜你，你猜对了");
    return 0;
}

