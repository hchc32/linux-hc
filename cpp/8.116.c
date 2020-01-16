#include <stdio.h>
#include<ctype.h>
char get_first(void);
int main()
{   
    char ch;
    ch=get_first();
    putchar(ch);
    return 0;
}
char get_first(void)
{
    int ch;
   while(isspace(ch=getchar()))
   {
        getchar();
        continue;
    }
    while(getchar()!='\n')
        continue;
    return ch;
}

