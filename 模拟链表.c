#include<stdio.h>
int main()
{
    int data[101],right[101];
    int i,len,j,t,n;
    scanf("%d",&n);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&data[i]);
    }
    len=n;
    for(i=1;i<=n;i++)             //
    {
        if(i!=n)                //
            right[i]=i+1;
        else
            right[i]=0;
    }
    len++;
    scanf("%d",&data[len]);
    //遍历链表
    t=1;
    while(t!=0)
    {
        if(data[right[t]]>data[len])      //
        {
            right[len]=right[t];
            right[t]=len;
            break;                       // 
        }
        t=right[t];
    }
    //输出链表中所有的数
    t=1;
    while(t!=0)
    {
        printf("%d ",data[t]);
        t=right[t];
    }
    return 0;
}
