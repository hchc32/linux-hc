#include <stdio.h>
void print(char ch,int n, int m);
int main()
{
    char ch;
    int n,m;
    scanf("%c %d %d",&ch,&n,&m);
    print(ch,n,m);
    return 0;
}
void print(char ch,int n, int m)
{
    int i,j;
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
            printf("%c",ch);
        }
        printf("\n");
    }
}

