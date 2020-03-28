#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        printf("I am child,will die!\n");
        sleep(2);
        //return 101;
        while(1)
        {
            printf("No die!HaHa\n");
            sleep(1);
        }
    }
    else if(pid > 0)
    {
        printf("I am parent,wait for child die!\n");
        int status;
        pid_t wpid = wait(&status);
        printf("wait ok,wpid = %d,pid = %d\n",wpid,pid);
        //判断死亡方式：正常死亡
        if(WIFEXITED(status))
        {
           printf("child exit with %d\n",WEXITSTATUS(status)); 
        }
        //信号杀死
        if(WIFSIGNALED(status))
        {
            printf("child killed by %d\n",WTERMSIG(status));
        }
        while(1)
        {
            sleep(1);
        }
    }
    return 0;
}

