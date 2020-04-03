#include <stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
int main(int argc,char* argv[])
{
    pid_t pid = fork();
    if(pid > 0)
    {
        while(1)
        {
            printf("parent process, %d\n",getpid());
            sleep(1);
        }
    }
    else if(pid == 0)
    {
        sleep(2);
        kill(getppid(),SIGKILL);

    }
    return 0;
}

