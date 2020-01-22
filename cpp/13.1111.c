#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#define LEN 10
int main(int argc,char *argv[])
{
    char filename[LEN];
    char ch;
    FILE *fp;
    if((fp=fopen(argv[2],"r"))!=NULL)
    {
        printf("file open error!");
    }
    while(fgets(filename,LEN,fp))
    {
        if(strstr(filename,argv[1]))
            fputs(filename,stdout);
    }
    fclose(fp);
    return 0;
}

