#include <stdio.h>

int main()
{
    char ch;
    int  k=0,h=0,e=0;
    do
    {
        scanf("%c",&ch);
        switch(ch)
        {
        case ' ':k++;
        case '\n':h++;
        default:e++;
        }
    }
    while(ch!='#');
    printf(" %d",k);
    printf("\\n%d",h);
    printf("other %d",e);


    return 0;
}

