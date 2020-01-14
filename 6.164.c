#include <stdio.h>

int main()
{
    int n=65,i,j,k=n;
    j=n;
    for(i=0;i<6;i++)
    {
        for(j=1;j<=i+1;j++)
            printf("%c",n++);
        printf("\n");
    }

    return 0;
}

