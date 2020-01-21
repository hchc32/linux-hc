#include <stdio.h>
#include<string.h>
char *find_str(char *p1,char *p2);
int main()
{
    char str1[1000],str2[1000];
    char *a;
    scanf("%s",str1);
    scanf("%s",str2);
    a=find_str(str1,str2);
    printf("%p",a);
    return 0;
}
char *find_str(char *p1,char *p2)
{
    int i;
    char *temp;
    int len2=strlen(p2);
    if(strlen(p1)-len2>0)
    {
        for(temp=p1;temp<p1+strlen(p1)-strlen(p2);temp++)
        {
            if(strlen(temp)<len2)
                return NULL;
            if(strncmp(temp,p2,len2))
            {
                return (char*)temp;
            }
        }
    }
}

