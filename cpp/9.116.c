#include <stdio.h>
void swap(double *x,double *y,double *z);
int main()
{
    double x,y,z;
    scanf("%lf %lf %lf",&x,&y,&z);
    printf("%lf %lf %lf\n",x,y,z);
    swap(&x,&y,&z);
    printf("%lf %lf %lf",x,y,z);
    return 0;
}
void swap(double *x,double *y,double *z)
{
    int a[3],i,j;
    a[0]=*x;
    a[1]=*y;
    a[2]=*z;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                int temp;
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
    *x=a[0];
    *y=a[1];
    *z=a[2];
}

