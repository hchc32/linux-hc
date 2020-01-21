#include <stdio.h>
int get_ch(char *str,char ch);
int main()
{
    char ch;
    char str[100];
    scanf("%s",str);
    scanf("%c",&ch);
    get_ch(str,ch);
    if(get_ch(str,ch)!=-1)
        printf("%d",get_ch(str,ch));
    return 0;
}
int get_ch(char *str,char ch)
{
    int i=0;
    while(*str!='\0')
    {
        if(*str==ch)
            return i;
        else
        {
            str++;
            i++;
        }
    }
    return -1;
}

