#include<stdio.h>
double  fun(double n1,double n2);
int main()
{
 double n1,n2;
 while(1)
 {
  if(scanf("%lf %lf",&n1,&n2)==2)
  {
   n1=fun(n1,n2);
   printf("%lf",n1);
  }
  else
   break;
 }
 return 0;
 }
double  fun(double n1,double n2)
{
 return (n1-n2)/n1*n2;
}



