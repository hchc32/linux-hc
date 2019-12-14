/*#include<stdio.h>
#include<string.h>
int main()
{
char a[100000];
int n;
do
{
scanf("%s",a);
if(a[0] =='0')
break;
else
{
int i;
int sum=10;
int n=strlen(a);
while(sum/10!=0)
{
sum=0;
for(i=n-1;i>=0;i--)
{
sum=sum+a[i]-48;
}
memset(a,'\0',sizeof(a));
int k;
n=sum;
for(i=0;n!=0;i++)
{
k=n%10;
a[i]=k+48;
n=n/10;
}
n=strlen(a);
}
printf("%s\n",a);
}
memset(a,'\0',sizeof(a));
}
while(1);
return 0;
}
#include<stdio.h>
#include<string.h>
int main()
{
char a[1000];
char s[20];
char e[10];
char str[]="ENDOFINPUT";
do
{
fgets(s,sizeof(s),stdin);
if(strcmp(s,str)==0)
return 0;
fgets(a, sizeof(a), stdin);
fgets(e,sizeof(e),stdin);
int i;
int n=strlen(a);
for(i=0;i<n;i++)
{
if(a[i]>=65 && a[i]<=90)
{
if(a[i]-5-65<0)
{
a[i]=90+a[i]-5-65+1;
}
else
{
a[i]=a[i]-5;

}
}
}
printf("%s",a);
memset(a,'\0',sizeof(a));
memset(s,'\0',sizeof(s));
memset(e,'\0',sizeof(e));
}while(1);
return 0;
}*/
#include<stdio.h>
#include<string.h>
int main()
{
	char b[][9]={"imix","ik","akbal","kan","chicchan","cimi","manik","lamat","muluk", "ok", "chuen","eb","ben","ix","mem","cib","caban","eznab","canac","ahau"};
	char d[][7]={"pop","no","zip","zotz","tzec","xul","yoxkin","mol", "chen", "yax","zac","ceh","mac","kankin","muan","pax","koyab","cumhu","uayet"};
	int N,k,i;
	char a1[10];
	int num,num1,sum;
	int flag;
	scanf("%d",&N);
	for(i=0;i<N;i++)
	{
		scanf("%d. %s %d",&num,a1,&num1);
	//	printf("%d. %s %d",num,a1,num1);
		for(i=0;i<19;i++)
		{
			if(strcmp(a1,d[i])==0)
				break;
		}
		sum=i*20+num+num1*365;
		num1=sum/260;
		flag=sum%20;
	//	printf("%d",sum);
		num=(sum%13)+1;
		printf("%d. %s %d\n",num,b[flag],num1);
		continue;
	}
	return 0;

}





