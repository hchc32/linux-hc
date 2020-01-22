#include <stdio.h>
#include<stdlib.h>
int numfile(FILE *fp,char ch);
int main(int agrc,char *argv[])
{   
    FILE *fp;
    char ch[100];
    scanf("%s",&ch);
    fp=fopen(ch,"wb");
    printf("%s出现的次数为%d",argv[1],numfile(fp,*argv[1]));
    return 0;
}

int numfile(FILE *fp,char ch)
{
    char find;
    int count=0;
    while((find=fgetc(fp))!=EOF&&!feof(fp))
    {
        if(ch==find)
        {
            count++;
        }
    }
    return count;
}
