#include <stdio.h>
void ds(double *a,int n);
int main()
{
    double a[1000];
    int n,i;
    scanf("%d",&n);
    for(i=0;i<n;i++)
        scanf("%lf",&a[i]);
    ds(a,n);
    for(i=0;i<n;i++)
        printf("%.0lf ",a[i]);
    return 0;
}
void ds(double *a,int n)
{
    int i,j;
    double *p;
    for(i=0;i<n/2;i++)
    {
        j=a[i];
        a[i]=a[n-i-1];
        a[n-i-1]=j;
    }
}
