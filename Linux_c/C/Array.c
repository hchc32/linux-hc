//下面这个程序是否有问题？

#include <stdio.h>
#define MAX 255

int main()
{
    unsigned char a[MAX],i;
    for(i=0;i<=MAX;i++)
        a[i] = i;
    for(i=0;i<=MAX;i++)
        printf("%d ",a[i]);
    printf("\n");
    return 0;
}

//编译可以通过，但是运行时候会发生数组越界和死循环
//unsigned char  的数据范围是0～255
//                           0000 0000 ～1111 1111
//i = 255时的下一步，数据依然要截断8位，又回到了0000 0000
//数组对越界访问不会检查，必须由程序员自己来检查
