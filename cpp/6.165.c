#include<stdio.h>
int main()
{
    char c;
    scanf("%c",&c);
    int i,j,k;
    // printf("%d",c);
    for(i=0;i<=c-65;i++)
    {
        for(k=i;k<=c-65;k++)
            printf(" ");
        for(j=65;j<=65+i;j++)
            printf("%c",j);
        for(j=j-2;j>=65;j--)
            printf("%c",j);
        printf("\n");
    }
    return 0;
} 
