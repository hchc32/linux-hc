#include<stdio.h>
#include<ctype.h>
int main()
{
 double n1,n2;
 while(1)
 {
  if(scanf("%lf %lf",&n1,&n2)==2)
  {
   printf("%lf\n",(n1-n2)/n1*n2);
  }
  else
   break;
 }
 return 0;
 } 
