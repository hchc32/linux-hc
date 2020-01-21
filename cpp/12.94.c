#include <stdio.h>
int count (void);
int main()
{
    int fun;
    scanf("%d",&fun);
    int i;
    for(i=0;i<fun;i++)
        printf("%d",count());
    return 0;
}
int count(void)
{
    static int count=1;
    return count++;

}

