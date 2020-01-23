#include <stdio.h>

int main()
{
    int n=70,i,j;
    for(i=0;i<6;i++)
    {
        for(j=n;j>=n-i;j--)
            printf("%c",j);
        printf("\n");
    }

    return 0;
}

