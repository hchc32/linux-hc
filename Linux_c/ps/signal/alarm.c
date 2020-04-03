#include <stdio.h>
#include<unistd.h>
int main()
{
    int ret = alarm(5);
    printf("ret = %d\n",ret);
    sleep(2);
    //重新设置定时器
    ret = alarm(2);
    printf("ret = %d\n",ret);
    while(1)
    {
        printf("hello , linux\n");
        sleep(2);
    }
}
