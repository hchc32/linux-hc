#include<stdio.h>         //冒泡排序成绩加名字
struct Student
{
    char name[20];
    int score;
};
int main()
{   
    struct Student a[20],temp;
    int i,n,j;
    scanf("%d",&n);
    for(i=0;i<n;i++)
    {
        scanf("%s  %d",a[i].name,&a[i].score);
    }
    for(i=0;i<n-1;i++)
    {
        for(j=0;j<n-i;j++)
        {
            if(a[j].score<a[j+1].score)
            {
                temp=a[j];
                a[j]=a[j+1];
                a[j+1]=temp;
            }
        }
    }
    for(i=0;i<n;i++)
    {
        printf("%s\n",a[i].name);
        getchar();
        getchar();
    }

    return 0;
}                                              
