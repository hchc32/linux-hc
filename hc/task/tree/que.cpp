#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int place(int s,int p[])
{
	for(int i = 1;i < s;i++)
		if(abs(i-s) == abs(p[i] - p[s]) || (p[i] == p[s]))
		return 0;
	return 1;
}

void print(int p[],int n)
{
    for(int i = 1;i <= n;i++)
    	printf("%d ",p[i]);
    printf("\n");
}
void Tria(int i,int n,int p[])
{
	if(i > n)
    {
		print(p,n);
    }
	else for(int j = 1;j <= n;j++)
    {
		p[i] = j;
		if(place(i,p))
			Tria(i+1,n,p);
	}
} 
int main(int argc , char *argv[])
{
	int n;
	int *bod = (int *)malloc(sizeof(int)*(n+1));
	printf("请输入皇后的个数:\n");
	scanf("%d",&n);
	Tria(1,n,bod);
	free(bod);
    return 0;
}
