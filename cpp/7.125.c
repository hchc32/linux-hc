#include <stdio.h>

int main()
{
    char ch;
    char ch1;
    int flag=0;
    while((ch=getchar())!='#')
    {
        if(ch=='e')
            ch1=ch;
        if(ch1=='e' && ch=='i')
            flag++;
    }
    printf("The total count is %d",flag);
    return 0;
}

