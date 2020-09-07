#include <stdio.h>
#include<string.h>
int main()
{

 char path[30];
 char *p1,*p2;
 char filename[20];
 char buff[1024];

 
    scanf("%s",path);
    p1 = path;
    p2 = path+1;
    printf("%c %c\n",*p1,*p2);
    for(int i = 0 ; i < strlen(path) ; i++)
    {
        printf("*p2=%c\n",*p2);
        if(*p2 == '/')
        {
            p1 = p2;
            p2++;
        }
        else
        {
            p2++;
        }
    }
    int i =0;
while(p1 != p2)
{
    printf("p1=%c\n",*p1);
    p1++;
    filename[i++] = *p1;
}
printf("filename %s\n",filename);

    return 0;
}

