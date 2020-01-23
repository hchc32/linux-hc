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
        if(ch=='.')
        {
            ch='!';
            count++;
        }
        if(ch=='!')
        {
            ch='!!';
            count++;
        }
    }
    while(1);
    printf("%d",count);
    return 0;
}

