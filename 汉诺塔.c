#include<stdio.h>                           
void  han(int N,char x,char y,char z);
int main()
{
        int n;
        scanf("%d",&n);
        han(n,'A','B','C');
        return 0;
}

void  han(int N,char x,char y,char z)
{
        if(N==1)
                printf("%c-->%c\n",x,z);
        else
        {
                han(N-1,x,z,y);
                printf("%c-->%c\n",x,z);
                han(N-1,y,x,z);
        }
}
