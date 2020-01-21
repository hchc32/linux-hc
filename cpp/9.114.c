#include <stdio.h>
double aver_th(double n,double m);
int main()
{
    double n,m;
    scanf("%lf %lf",&n,&m);
    printf("%.2lf",aver_th(n,m));
    

    return 0;
}

double aver_th(double n,double m)
{
    return (1/n+1/m)/2;
}
