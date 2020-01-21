#include <stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    double n1,n2,sum=1;
    int i;
    n1=atof(argv[0]);//转化为浮点数
    n2=atoi(argv[1]);//转化为整形数
    for(i=0;i<n2;i++)
        sum*=n1;
    printf("%lf",sum);
    return 0;
}

