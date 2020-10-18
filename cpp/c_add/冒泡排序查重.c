#include<stdio.h>
int main()
{
    int a[100],i,j,n,temp;
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(a[j]>a[j+1])
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    //  if(flag == 0)
    //      break;
    }
    printf("%d",a[0]);
    for(i=1;i<n;i++)
    {
        if(a[i]!=a[i-1])
        {
            printf(" %d",a[i]);
        }
    }
    getchar();
    return 0;
}                                
