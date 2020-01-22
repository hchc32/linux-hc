#include <stdio.h>
#define AVE_TH(X,Y) (1.0/(((1.0/X)+(1.0/Y))/2))
int main()
{
    double x,y;
    scanf("%lf%lf",&x,&y);
    printf("调和平均数为:%lf",AVE_TH(x,y));
    return 0;
}

