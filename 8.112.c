#include <stdio.h>

int main()
{
    char ch;
    int flag=0;
    while(ch=getchar()!=EOF)
    {
        if(ch=='\n')
        {
            putchar('\\');
            printf("n");
            printf("%d",ch);
            flag++;
        }
        else if(ch=='\t')
        {
            putchar('\\');
            printf("t");
            printf("%d",ch);
            flag++;
        }
        else
        {
            putchar(ch);
            printf("%d",ch);
            flag++;
        }
        if(flag%10==0)
            printf("\n");
    }
    return 0;
}

