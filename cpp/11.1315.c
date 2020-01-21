#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define SIZE 100
double atoi(char * st);
int main()
{
    char num[SIZE];
    double sum;
    fgets(num,5,stdin);
    sum=atoi(num);
    printf("%.2lf",sum);
    return 0;
}
double atoi(char * st)
{
    int i;
    int len=strlen(st);
    double result = 0;
    for(i = 0; i < len; i++)
    {
        if(isdigit(st[i]))//是否为10进制字符
        {
            result += ((st[i])*pow(10,(len-i-1)));
        }
        else
            return 0;
    }
    return result;
}
