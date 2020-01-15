#include <stdio.h>

int main()
{
    char c;
    int flag=0;
    while((c=getchar())!='#')
    {
        switch(c)
        {
            case '.':
                    {
                        c='!';
                        flag++;
                        break;
                    }
            case '!':
                    {
                        putchar(c);
                        flag++;
                        break;
                    }
            default:
                break;
        }
        putchar(c);
    }
    printf("经历了%d次代换!",flag);
    return 0;
}

