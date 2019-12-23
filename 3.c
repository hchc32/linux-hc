#include<stdio.h>
void max_min (int a[],int n,int *max,int *min);
int main()
{
    int i,a[10];
    int max,min;
    printf("Please input integer numbers you want to oerate:\n ");
    for(i=0;i<10;i++)
        scanf("%d",&a[i]);
    max_min(a,10,&max,&min);
    printf("The maximum number is  : %d \n",max);
    printf("The minimum number is  : %d \n",min);
    getchar();
    return 0;
}
void max_min (int a[],int n,int *max,int *min)
{
    int *p;
    *max=*min=*a;
    for(p=a+1;p<a+n;p++)
    {
        if(*p>*max)
            *max=*p;
        if(*p<*min)
            *min=*p;
    }
}
