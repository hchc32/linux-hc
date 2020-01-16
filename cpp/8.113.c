#include <stdio.h>
#include<ctype.h>
int main()
{
    char ch;
    int count1=0,count2=0;
    while((ch=getchar())!=EOF)
    {
        if(islower(ch))
            count1++;
        if(isupper(ch))
            count2++;
    }
    printf("小写字母个数为:%d个\n",count1);
    printf("大写字母个数为:%d个\n",count2);
    return 0;
}

