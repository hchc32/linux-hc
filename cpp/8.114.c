#include <stdio.h>
#include<ctype.h>
int main()
{
    char ch;
    double count1=0,word=0;
    int flag=1,flag1=0;
    while((ch=getchar())!=EOF)
    {
        if(isalpha(ch))
        {
            count1++;
            flag1=1;
            continue;
        }
        else
        {
            word=word+flag1;
            flag1=0;
        }
    }
    if(flag1==1)
        word++;
    printf("%lf %lf",count1,word);
    printf("平均每个单词字母个数为%.0lf个\n",count1/word);
    return 0;
}

