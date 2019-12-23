#include <stdio.h>

int main()
{
    char ch;
    int count=0;
    do
    {
        scanf("%c",&ch);
        if(ch=='#')
            break;
        printf("%c %d  ",ch,ch);
        count++;
        if(count%8==0)
            printf("\n");
    }
    while(1);
    return 0;
}

