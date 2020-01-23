#include <stdio.h>

int main()
{
    int old;
    printf("请输入你的年龄:");
    scanf("%d",&old);
    printf("对应的秒数为%.3f\n",old*3.156*(10e7));
    return 0;
}

