#include <stdio.h>
int fun(int n);
int main()
{
    int n,a,b,c,sum=0;
    printf("请输入火柴棍的数目(<=24)：");
    scanf("%d",&n);   //火柴的数目
    for(a=0;a<=1111;a++)
    {
        for(b=0;b<=1111;b++)
        {
            c=a+b;
            if(fun(a)+fun(b)+fun(c)==n-4)
            {
                printf("%d+%d=%d\n",a,b,c);
                sum++;
                          
            }
        }
    }
    printf("一共可以拼出%d个等式",sum);
    getchar();
    getchar();
    return 0;
}

int fun(int n)
{
    int a[10]={6,2,5,5,4,5,6,3,7,6};
    int num=0,i;
    if(n==0)
        return a[0];
    while(n!=0)
    {
        i=n%10;
        n=n/10;
        num=num+a[i];
    }
    return num;
}
