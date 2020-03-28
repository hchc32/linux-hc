#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        printf("I am child,pid = %d\n",getpid());
        sleep(2);
    }
    else if(pid > 0)
    {
        printf("I am parent,pid = %d\n",getpid());
        int ret = waitpid(-1,NULL,WNOHANG);
        printf("ret = %d\n",ret);
        while(1)
        {
            sleep(1);
        }
    }
    return 0;
}

