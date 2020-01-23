#include <stdio.h>
#define DAP 0.1
#define DER 0.05
int main()
{
    double dap=100,der=100,sum=100;
    int year=0;
    while(dap>=der)
    {
        dap+=100*DAP;
        der+=der*DER;
        year++;
    }
    printf("需要经过%d 年",year);
    printf("DAP=%.2lf",sum);
    printf("DER=%.2lf",der);

    return 0;
}

