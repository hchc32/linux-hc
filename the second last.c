#include<stdio.h>                                                                
#include<string.h>
int main()
{
    char str1[100000],str2[100000];
    int j,i,i1,i2,flag1,flag2,flag1_,flag2_;
    int m,n;
    while(scanf("%s %s",str1,str2)!=-1)
    {
        flag1=0;flag2=0;flag1_=0;flag2_=0;
        i1=0;   i2=0;
        int flag3=1;
        m=strlen(str1);
        n=strlen(str2);
        for(i=0;i<m;i++)
        {
            if(str1[i]=='.')
            {
                i1=1;
            }
        }

        if( i1 == 0 )
        {
            str1[i]='.';
            ++m;
        }
//      printf("%c\n",str1[i-1]);
        for(i=0;i<n;i++)
        {
            if(str2[i]=='.')
            {
                i2=1;
            }
        }
        if( i2 == 0 )
        {
            str2[i]='.';
            ++n;
        }
//      printf("\n%c\t",str2[i]);
//      printf("\n%c\t",str2[i-1]);
        for(i=0;i<m;i++)
        {
            if(str1[i]!='0')
            {
                flag1=i;
                break;
            }
        }
        for(i=m-1 ;i>=0 ;i-- )
        {
            if(str1[i]!='0')
            {
                flag1_=i;
                break;
            }
        }
        for(i=0;i<n;i++)
        {
            if(str2[i]!='0')
            {
                flag2=i;
                break;
            }
        }
        for(i=n-1 ;i>=0 ;i-- )
        {
            if(str2[i]!='0')
            {
                flag2_=i;
                break;
            }
        }
//      printf("%d %d",flag1,flag1_);
        if((flag1_-flag1)!=(flag2_-flag2))
        {
            printf("NO\n");
            continue;
        }

        int s=flag1_-flag1+1;
            for(i=0;i<s;i++)            //注意循环体的位置！！！
            {
                if(str1[flag1++]!=str2[flag2++])
                {
                    printf("NO\n");
                    flag3=0;
                    break;
                }
            }
//          printf("%d",flag3);
            if(flag3!=0)
            {
//      printf ("%d %d",str1[flag1],str2[flag2]);
            printf("YES\n");
            }
    }
    memset(str1,'\0',sizeof(str1));
    memset(str2,'\0',sizeof(str2));
    return 0;
}
