#include <stdio.h>

int main()
{
    int n,m,k;
    scanf("%d",&n);
    while(1)
    {
        scanf("%d",&m);
        if(m<=0)
            break;
        else
            printf("%d %% %d is %d\n",m,n,m%n);
    }

    return 0;
}

