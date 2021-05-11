#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    char str[1000] = "GET /562f25980001b1b106000338.jpg HTTP/1.1";
    char content[1000] = "Content-Length:40";
    char *content_lenght = content;
    char *murl;
    murl = strpbrk(str," \t");
    *murl++ = '\0';
    if(strncasecmp(content_lenght,"Content-length:",15)==0)
    {
        content_lenght+=15;
        content_lenght+=strspn(content_lenght," \t");
        printf("%d\n",atol(content_lenght));
    }
    //printf("%s\n",murl);
    return 0;
}

