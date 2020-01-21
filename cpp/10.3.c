#include <stdio.h>
int max(int a[],int n);
int main()
{
    int a[1000];
    int n,i;
    scanf("%d",&n);
    for(i=0;i<n;i++)
        scanf("%d",&a[i]);
    printf("%d\n",max(a,n));
    return 0;
}
int max(int a[],int n)
{
    int max=-1;
    for(int i=0;i<n;i++)
    {
        if(a[i]>max)
            max=a[i];
    }
    return max;
}

