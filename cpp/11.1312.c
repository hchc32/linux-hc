#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define  SIZE 100
int main()
{
    char str[SIZE];
    int num1,i,j,num2,words,puncs,nums;
    int len;
    num1=num2=puncs=nums=0;
    printf("请输入字符串:\n");
    fgets(str, SIZE, stdin);
    len = strlen(str);
    for(i=0;i<len;i++)
    {
        if(isalpha(str[i]))
        {
            if(str[i] >= 'a' && str[i] <= 'z')
                num2++;
            else
                num1++;
            words++;
        }
        else if(ispunct(str[i]))
            puncs++;
        else if(isdigit(str[i]))
            nums++;
    }
    return 0;
}
