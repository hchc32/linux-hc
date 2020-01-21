#include <stdio.h>
void copy_arr(double* target[5], double* source[5], int n,int m);
int main()
{
    double a[1000][1000],b[1000][1000];
    int n,m,i,j;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
            scanf("%lf",&a[i][j]);
    }
    copy_arr(a,b,5,5);
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
            printf("%lf",a[i][j]);
    }
    
    return 0;
}
void copy_arr(double* target[5], double* source[5],int n,int m)
{
    int i,j;
    for (i = 0; i < n; i++)
        for(j=0;j<m;j++)
        {
            target[i][j] = source[i][j];
        }
    for(i=0;i<n;i++)
        for(j=0;j<m;j++)
            printf("%lf",target[i][j]);
}
