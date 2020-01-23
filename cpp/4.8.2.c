#include <stdio.h>

int main()
{
    char n1[10],n2[10];
    printf("请输入你的名和姓:");
    scanf("%s,%s",n1,n2);
    printf("\"%s,%s\"",n1,n2);
    printf("\"%20s,%20s\"",n1,n2);
    printf("\"%-20s,%-20s\"",n1,n2);
    printf("\"%-20s,%-20s\"",n1,n2);
    return 0;
}

