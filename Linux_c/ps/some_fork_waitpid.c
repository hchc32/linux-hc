#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    pid_t pid;
    int i;
    for( i = 0; i < 5; i++ )
    {
        pid =fork();
        if(pid == 0)
        {
            break;
        }
    }
    if(i < 5)
    {
        sleep(i);
        printf("I am child! i = %d, pid = %d\n",i,getpid());
    }
    if(i == 5)
    {
        //使用waitpid()函数回收
        while(1)
        {
            pid_t wpid = waitpid(-1,NULL,WNOHANG);
            if( wpid == -1)
            {
                break;
            }
            else if(wpid > 0)
            {
                printf("wpid = %d\n",wpid);
            }
        }
        while(1)
        {
            sleep(1);
        }
    }

    return 0;
}

