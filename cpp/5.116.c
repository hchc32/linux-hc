#include <stdio.h>

int main()
{
    int count,sum;
    int n;
    scanf("%d",&n);
    count = 0;
    sum = 0;
    while(count++ < n)
        sum = sum + count * count;
    printf("sum = %d\n",sum);
    return 0;
}
