#include <stdio.h>
double large_of(double *x,double *y);
#define COMPARE 1e-6
int main()
{
    double x,y;
    scanf("%lf %lf",&x,&y);
    printf("%lf %lf\n",x,y);
    large_of(&x,&y);
    printf("%lf %lf\n",x,y);
    return 0;
}
double large_of(double *x,double *y)
{
    if(*x-*y>COMPARE)
    {
        *y=*x;
    }
    else
    {
        *x=*y;
    }

}
