#include <stdio.h>

int main()
{
    char ch;
    int flag=0;
    while(ch=getchar()!=EOF)
        ++flag;
    printf("输入的字符数为 %d 个",flag);
    return 0;
}

