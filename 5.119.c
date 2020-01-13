#include <stdio.h>

void Temperatures(double hs_t);
int main()
{
    double temper;
    while(1)
    {
        scanf("%lf",&temper);
        if(temper == 'q' || temper<0)
            break;
        else
            Temperatures (temper);
    }
    return 0;
}
void Temperatures(double hs_t)
{
    printf("摄氏温度 = %.2lf\n",5.0/9.0*(hs_t - 32.0));
    printf("开氏温度 = %.2lf\n",5.0/9.0*(hs_t - 32.0)+273.16);
    return ;
}

