#include <stdio.h>
double fun(double n);
int main()
{
    double n;
    scanf("%lf",&n);
    n=fun(n);
    printf("%lf",n);
    return 0;
}
double fun(double n)
{
    double s;
    s=n*n*n;
    return s;
}

