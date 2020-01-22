#include <stdio.h>
#include<stdlib.h>
int main()
{
    FILE *fp,*fp1;
    int a[20][30];
    char str[20][31];
    char filename1[20];
    char filename2[20];
    int i,j;
    scanf("%s",filename1);
    scanf("%s",filename2);
    if((fp=fopen(filename1,"wb"))==NULL)
    {
        printf("filename1 open error!");
        exit(-1);
    }
    if((fp=fopen(filename2,"wb"))==NULL)
    {
        printf("filename2 open error!");
        exit(-1);
    }
    for(i=0;i<20;i++)
    {
        for(j=0;j<30;j++)
        {
            str[i][j]=
        }
        str[i][j]='\0';
        fprintf(fp1,"%s\n",str[i]);
    }
    fclose(fp);
    fclose(fp1); 
    return 0;
}
