#include <stdio.h>
#include<string.h>
char *s_gets(char *st,int n);//n是字符串长度
int laststr(char *s);
int main()
{
    char str[1000];
    printf("请输入字符串:");
    while(s_gets(str,10) && str[0]!='\0')
    {
        printf("处理后的字符串如下:");
        laststr(str);
        printf("%s",str);
    }
    return 0;
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

int laststr(char *s)
{
    int i;
    int len=strlen(s);
    while(*s!='\0')
    {
        for(i=0;i<len;i++)
        {
            if(*(s+i)==' ')
            {
                *(s+i)=*(s+i+1);
            }
        }
        return 1;
    }
}
