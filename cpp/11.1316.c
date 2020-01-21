#include <stdio.h>
#include<string.h>
#include<ctype.h>
#define LEN 100
int main(int argc,char *argv[])
{
    char str[LEN];
    int i,j,len;
    fgets(str,10,stdin);
        len=strlen(str);
        if(strcmp(argv[1],"-u")==0)
        {
            for(i=0;i<len;i++)
            {
                str[i]=toupper(str[i]);
            }
            fputs(str,stdout);
        }
        else if(strcmp(argv[1],"-l")==0)
        {
            for(i=0;i<len;i++)
            {
                str[i]=tolower(str[i]);
            }
            fputs(str,stdout);
        }
        else
            fputs(str,stdout);
    return 0;
}

