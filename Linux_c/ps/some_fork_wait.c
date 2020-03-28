#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    pid_t pid;
    int i = 0;
    for(  i = 0; i < 5; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            printf("I am child , pid = %d\n",getpid());
            break;
        }
    }
    sleep(i);
    if(i  == 5 )
    {
         for( int i = 0 ;i < 5; i++)
          {
            pid_t wpid = wait(NULL);
            printf("wpid = %d\n ",wpid);
          }
        while(1)
        {
            sleep(1);
        }
    }
    return 0;
}

