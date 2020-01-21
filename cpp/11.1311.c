#include <stdio.h>
#include<string.h>
#include<ctype.h>
char *s_gets(char *st,int n);//n是字符串长度
void print_up(char *str);
void asc(char *str);
void print(char *str);
void print_first_len(char *str);
int main()
{
    char str[100],select;
    s_gets(str,5);
    printf("**********************************************\n");
    printf("|         请选择你想做的事：              |\n");
    printf("|  a.顺序打印字符       b.打印ASCII码     |\n");
    printf("|  c.长度递增打印       d.第一个单词长度打印|\n");
    printf("|             q.不玩了                  |\n");
    printf("**********************************************\n");
    scanf("&c",&select);
    while(select!= 'q')
    {
        switch(select)
        {
            case 'a':
                print(str);
                break;
            case 'b':
                asc(str);
                break;
            case 'c':
                print_up(str);
                break;
            case 'd':
                print_first_len(str);
                break;
            default:
                printf("请输入正确选项：");
                scanf("%c",&select);
                continue;
        }
        printf("----------------------------------------\n");
        printf("|         请选择你想做的事：              |\n");
        printf("|  a.顺序打印字符       b.打印ASCII码     |\n");
        printf("|  c.长度递增打印       d.第一个单词长度打印|\n");
        printf("|             q.不玩了                  |\n");
        printf("----------------------------------------\n");
        scanf("%c",&select);
        getchar();
    }
    return 0;
}

void asc(char *str)
{
    int i;
    int len=strlen(str);
    for(i=0;i<len;i++)
    {
        printf("%d ",str[i]);
    }
}

void print_up(char *str)
{
    int i,j;
    int len=strlen(str);
    for(i=0;i<len;i++)
    {

    }
}

char *s_gets(char *st,int n)//n是字符串长度
{
    char *str,*find;
    str=fgets(st,n,stdin);
    if(st)
    {
        find=strchr(st,'\n');  //看字符串中有没有\n
        if(find)     //找到'\n'->'\0'
        {
            *find='\0';   
        }
        else
            while(getchar()!='\n')
                continue;
    }
    return str;
}
void print(char *str)
{
    fputs (str,stdout);
}
void print_first_len(char *str)
{
    int i,j;
    int len=strlen(str);
    for(i=0;i<len;i++)
    {
        for(j=i;j<len;j++)
        {
            if(isspace(str[j]))
            {
                str[j]='\0';
                break;
            }
            break;
        }
    }
    printf("%s",str);
}
