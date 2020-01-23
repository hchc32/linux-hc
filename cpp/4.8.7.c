#include <stdio.h>
#include<float.h>
int main()
{
    double n;
    float m;
    n=1.0/3.0;
    printf("%.6lf\n",n);
    printf("%.12lf\n",n);
    printf("%.16lf\n",n);
    m=1.0/3.0;
    printf("%.6f\n",m);
    printf("%.12f\n",m);
    printf("%.16f\n",m);
    printf("FLT_DIG = %d\n",FLT_DIG);
    printf("DBL_DIG = %d\n",DBL_DIG);
    return 0;
}

