#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t pid;
    int x = 1;
    pid = fork();
    if(pid < 0)
    {
        exit(0);
    }
    else if(pid == 0)
    {
        printf("child: %d \n",++x);
    }
    else
    {
        printf("father: %d \n",x);
    }
    
    return 0;
}

