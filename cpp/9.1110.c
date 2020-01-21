#include <stdio.h>
void jz(int n,int m);
int main()
{
    int n,m;
    scanf("%d %d",&n,&m);
    if(m<2||m>10)
        printf("请重新输入数据\n");
    else
    {
        jz(n,m);
    }
    return 0;
}
void  jz(int n,int m)
{
    int flag;
    flag=n%m;
    if(n>=m)
    {
        jz(n/m,m);
    }
    printf("%d",flag);
}

