#include<stdio.h>
int main()
{
 int up,down;
 long sum=0;
 while(1)
 {
  sum=0;
  printf("Enter lower and upper integer limits:");
  scanf("%d %d",&up,&down);
  int n1=up*up;
  if(up<down)
  {
   for(up=up*up;up<=down*down;up++)
   {
    sum=sum+up;
   }
   printf("The sums of the squares from %d to %d is %ld",n1,down*down,sum);
  }
  else
  {
   printf("Done");
   break;
  }
 }
 return 0;
}
