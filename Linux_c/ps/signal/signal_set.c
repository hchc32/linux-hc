#include <stdio.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
int main(int argc,char* argv[])
{
    //屏蔽信号
    //先写自定义信号集
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGINT);//屏蔽ctrl+c
    sigaddset(&set,SIGQUIT);//屏蔽ctrl+反斜杠
    sigaddset(&set,SIGKILL);//９号信号不会被屏蔽，阻塞，的
    //将自定义信号集的数据同步到内核中的阻塞信号集
    sigprocmask(SIG_BLOCK,&set,NULL);
    //每隔1s获取一次未决信号集
    while(1)
    {
        sigset_t myset;
        //读取当前未决信号集   
        sigpending(&myset);
        for(int i = 1; i < 32; i++)
        {
            //对每一个信号进行判断
            if(sigismember(&myset,i))
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
        printf("\n");
        sleep(1);
    }
    return 0;
}

