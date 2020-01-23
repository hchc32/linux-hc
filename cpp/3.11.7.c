#include <stdio.h>

int main()
{
    int t;
    printf("请输入身高(英寸):");
    scanf("%d",&t);
    printf("对应的身高（厘米）:%.2f",2.54*t);
    return 0;
}

