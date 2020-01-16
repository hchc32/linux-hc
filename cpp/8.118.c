#include <stdio.h>
float a,b;
int main()
{
    char select='\0';
    while(1)
    {
        printf("Enter the operation of your choice:\n");
        printf("a . add                 s. subtract\n");
        printf("m . multiply            d .divide  \n");
        printf("q. quit\n");
        scanf("%c",&select);
        switch(select)
        {
            case 'a':
                {
                    scanf("%f %f",&a,&b);
                    printf("%.2f\n",a+b);
                    getchar();
                    getchar();
                    break;
                }
            case 's':
                {
                    scanf("%f %f",&a,&b);
                    printf("%.2f\n",a-b);
                    getchar();
                    getchar();
                    break;
                }
            case 'm':
                {
                    scanf("%f %f",&a,&b);
                    printf("%.2f",a*b);
                    getchar();
                    getchar();
                    break;
                }
            case 'd':
                {
                    scanf("%f %f",&a,&b);
                    if(b==0)
                    {
                        printf("请输入正确的数字:");
                        getchar();
                        getchar();
                        continue;
                    }
                    else
                    {
                        printf("%.2f",a*1.00/b);
                        getchar();
                        getchar();
                        break;
                    }

                }
            case 'q':
                {
                    return 0;
                }
        }
    }

    return 0;
}

