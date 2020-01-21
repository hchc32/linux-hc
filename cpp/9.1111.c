#include <stdio.h>
long feibo(int n);
int main()
{
    int N;
    printf("请输入你要知道的第几个斐波那契数:");
    scanf("%d",&N);
    printf("%ld",feibo(N));
    return 0;
}
long feibo(int n)
{
    if(n<=2)
        return 1;
    if(n>2)
        return feibo(n-1)+feibo(n-2);
}

