#include <stdio.h>
#include<stdlib.h>
void funa(FILE *p1,FILE *p2);
void funb(FILE *p1,FILE *p2);
int main(int argc,char *argv[])
{
    FILE *fp1,*fp2;
    if((fp1=fopen(argv[1],"r"))==NULL)
    {
        printf("fp1_file open error!");
        exit(-1);
    }
    if((fp2=fopen(argv[2],"r"))==NULL)
    {
        printf("fp2_file open error!");
        exit(-1);
    }
    printf("a:\n");
    funa(fp1,fp2);
    rewind(fp1);
    rewind(fp2);
    printf("b:\n");
    funb(fp1,fp2);
    return 0;
}

void funa(FILE *p1,FILE *p2)
{
    char ch1,ch2;
    do
    {   
        while((ch1=fgetc(p1)!=EOF && ch1!='\n'))
        {
            fputc(ch1,stdout);
            printf("\n");// 用来跳出循环
        }
        while((ch2=fgetc(p1)!=EOF && ch2!='\n'))
        {
            fputc(ch2,stdout);
            printf("\n");
        }
    }
    while(ch1!=EOF && ch2!=EOF);

}

void funb(FILE *p1,FILE *p2)
{
    char ch1,ch2;
    do
    {
        while((ch1=fgetc(p1)!=EOF && ch1!='\n'))
        {
            fputc(ch1,stdout);
        }
        while((ch2=fgetc(p1)!=EOF && ch2!='\n'))
        {
            fputc(ch2,stdout);
        }
        printf("\n");
    }while(ch1!=EOF && ch2!=EOF);
}
