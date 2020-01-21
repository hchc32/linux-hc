#include <stdio.h>
int is_within(char *str,char ch);
int main()
{
    char ch;
    char str[100];
    scanf("%s",str);
    scanf("%c",&ch);                                     
    is_within (str,ch);
    if(is_within (str,ch)!=-1)
        printf("%d",is_within (str,ch));
    return 0;
}
int is_within(char *str,char ch)
{
    int i=0;
    while(*str!='\0')
    {
        if(*str==ch)
            return 1;
        else
        {
            str++;
            i++;
        }
    }
    return 0;
}

