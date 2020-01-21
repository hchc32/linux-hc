#include <stdio.h>
int main(int argc,char *argv[])
{
    int i;

    for(i=1;i<argc;i++)
        printf("%s ",argv[argc-i]);
    return 0;
}
