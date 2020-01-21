#include <stdio.h>
#include<stdlib.h>
#include<ctype.h>
int main()
{
        FILE *fp;
        char file[20];
        char ch;
        printf("please input filename:");
        scanf("%s",file);
        if((fp=fopen(file,"wt"))==NULL)
        {
            printf("fpfile open error!");
            exit(-1);
        }
        while((ch=fgetc(fp))!=EOF)
        {
            if(islower(ch))
            {
                ch=ch+'45';
                fprintf(fp,"%c",ch);
            }
        }
        if(fclose(fp)!=0)
        {
            printf("close error!");
        }

    return 0;
}

