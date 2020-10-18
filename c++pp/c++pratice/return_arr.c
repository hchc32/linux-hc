#include<stdlib.h>
#include<stdio.h>
#include<time.h>

int* getrandom()
{
    //c++不支持在函数外部返回局部变量的地址，除非定义为static
    int r[10];
    //设置种子
    srand(time(NULL));
    for(int i = 0; i < 10 ; i++)
    {
        r[i] = rand();
        printf("%d\n",r[i]);
    }
    return r;
}

int main()
{
    int* p = getrandom();
    for(int i = 0; i < 10 ;i++)
    {
        printf("*(p+%d) = %d\n",i,*(p+i));
    }

    return 0;
}
