#include<stdio.h>
void quicksort(int left,int right);
int a[100],n;
int main()
{
    int i,j;
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%d",&a[i]);
    }
    quicksort(0,n-1);          //快排函数调用   注意传的值n-1,因为数组从0号元素开始的
    for(i=0;i<n;i++)
    {
        printf("%d ",a[i]);
    }
    return 0;
}
        //快排函数
void quicksort(int left,int right)
{
    int i,j,t,temp;
    if(left > right)
        return;
    i = left;
    j = right;
    temp=a[left];
    while(i != j)
    {
        while(a[j] >= temp && i < j)
        {
            j--;
        }
        while(a[i] <= temp && i < j)     //注意等于的时候也要移动标兵！！！ 
        {
            i++;
        }

        //交换标兵的位置
        if(i < j)
        {
            t=a[i];
            a[i]=a[j];
            a[j]=t;
        }
    }

    //基准数的交换
    a[left]=a[i];
    a[i]=temp;

    //分割后继续对部分进行递归排序
    quicksort(left,i-1);
    quicksort(i+1,right);
    return;
}
