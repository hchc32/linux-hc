#include<stdio.h>
int tj(int N);
int main()
{
	int s,n;
	scanf("%d",&n);
	s=tj(n);
	printf("%d",s);
	return 0;
}
int tj(int N)
{
	int sum=0,sum1=0,sum2=0,sum3=0;
	if(N==1||N==0)
		return 1;
	if(N>=1)
		sum1=tj(N-1);
    if(N>=2)
		sum2=tj(N-2);
	if(N>=3)
		sum3=tj(N-3);
	sum=sum1+sum2+sum3;
	return (sum);
}
