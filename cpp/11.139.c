#include <stdio.h>
#include<string.h>
void re_order(char* st);
int main()
{
    char str[1000];
    int i;
    fgets(str,100,stdin);
    getchar();
    re_order(str);
    for(i=0;i<strlen(str);i++)
        printf("%c",str[i]);
    return 0;
}
void re_order(char* st)
{
    int i;
    int n;
    int temp;

    n = strlen(st);
    for (i = 0; i < n / 2; i++)
    {
        temp = st[i];
        st[i] = st[n - i - 1];
        st[n - i - 1] = temp;
    }
}

