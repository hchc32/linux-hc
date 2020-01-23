#include <stdio.h>
#include<stdlib.h>
int main()
{
    int n;
   // float m=3.0*10e-23;
    printf("请输入水的夸脱数:");
    scanf("%d",&n);
    printf("对应水的质量为%.2f\n",(n*950)/(3.0*10e-23));
    return 0;
}

