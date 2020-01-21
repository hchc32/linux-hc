#include <stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    FILE *fp;
    char ch;
    for(int i=0;i<argc;i++)
    {
        if((fp=fopen(*argv,"wr"))==NULL)
        {
            printf("fpfile error!");
            continue;
        }
        while((ch=getc(fp))!=EOF)
            printf("%c",ch);
        if(fclose(fp))
        {
            printf("%sfile close erroe!",*argv);
            exit(0);
        }
    }

    return 0;
}

