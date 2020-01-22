#include <stdio.h>

int main()
{
    char filename[20];
    char ch;
    FILE *fp;
    int num;
    printf(" 请输入文件名称:\n");
    scanf("%s",filename);
    if((fp=fopen(filename,"wr"))==NULL)
    {
        printf("filename open error!i");
    }
    printf("请输入一个数字:");
    while(scanf("%d",&num)!=EOF)
    {
        while((ch=fgetc(fp))!='\n')
        {
            fprintf(stdout,"%s",&ch);
        }
    }
    return 0;
}

