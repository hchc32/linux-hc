#include <stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    FILE *fpin,*fpout;
    char ch;
    if((fpin=fopen(argv[1],"wb"))==NULL)
    {
        printf("infile open error!");
        exit(-1);
    }
    if((fpout=fopen(argv[2],"wb"))==NULL)
    {
        printf("outfile open error!");
        exit(-1);
    }
    while((ch=fgetc(fpin)!=EOF)&&!feof(fpin))
    {
        fprintf (fpout,"%d",ch);
    }
    fclose(fpin);
    fclose(fpout);
        return 0;
}

