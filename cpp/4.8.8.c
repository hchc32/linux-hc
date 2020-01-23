#include <stdio.h>
#define JL 3.785
#define YL 1.609
int main()
{
    float km,gas,yl,jl;
    printf("请输入里程数:");
    scanf("%f",&km);
    printf("请输入消耗汽油量:");
    scanf("%f",&gas);
    yl=km/YL;
    jl=gas/JL;
    printf("每加仑汽车行驶的英里数:");
    printf("%.1f\n",yl/jl);
    printf("每100公里耗油的升数:");
    printf("%.1f\n",gas/km*100);
    return 0;
}

