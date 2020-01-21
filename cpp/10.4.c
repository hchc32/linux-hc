#include <stdio.h>
double max(double a[],int n);
int main()
{
    double a[1000];
    int n,i;
    scanf("%d",&n);
    for(i=0;i<n;i++)
        scanf("%lf",&a[i]);
    printf("%d\n",max(a,n));
    return 0;                                          
}
double max(double a[],int n)
{
    double max=-1;
    int temp;
    for(int i=0;i<n;i++)
    {
        if(a[i]>max)
        {
            max=a[i];
            temp=i;
        }
    }
    return temp;
}

