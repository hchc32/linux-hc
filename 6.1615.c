#include <stdio.h>
#include<string.h>
int main()
{
    char str[255];
    int i;
    do
    {
        scanf("%c",&str[i]);
        if(str[i]=='\n')
        {
            str[i]='\0';
            break;
        }
        else
            i++;
    }
    while(1);
    for(i=strlen(str)-1;i>=0;i--)
        printf("%c",str[i]);
    return 0;
}

