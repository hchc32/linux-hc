#include <stdio.h>

int main()
{
    char name[10];
    float tail;
    printf("请输入身高(英寸)和姓名:");
    scanf("%f",&tail);
    scanf("%s",name);
    printf("%s ,you are %.3f feet tail",name,tail/2.54);


    return 0;
}

