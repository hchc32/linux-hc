#include <stdio.h>

int main()
{
    float n;
    printf("请输入杯数:");
    scanf("%f",&n);
    printf("品托数：%.2f",n/2);
    printf("盎司数：%.2f",n*8);
    printf("汤勺数：%.2f",n*8*2);
    printf("茶勺数：%.2f",n*8*2*3);
    return 0;
}

