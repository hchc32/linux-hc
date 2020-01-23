#include <stdio.h>
double findmin(double a,double b);
int main()
{
    double a,b;
    scanf("%lf %lf",&a,&b);
    printf("%.2lf",findmin(a,b));
       return 0;
}
double findmin(double a,double b)
{
    double min;
    min=a>b?b:a;
    return min;
}
