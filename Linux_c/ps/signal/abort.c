#include <stdio.h>
#include<signal.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>
int main()
{
    pid_t pid = fork();
    if(pid > 0)
    {
        //父进程
        int s;
        pid_t wpid = wait(&s);
        printf("child died pid = %d\n",wpid);
        if(WIFSIGNALED(s))
        {
            printf("died by signal: %d\n",WTERMSIG(s));
        }
    }
    else if(pid == 0)
    {
        while(1)
        {
            abort();
        }
    }
    return 0;
}
