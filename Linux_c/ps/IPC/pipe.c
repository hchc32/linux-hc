//pipe
#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error!\n");
        exit(1);
    }
    pid_t pid = fork();
    printf("%d\n",fd[0]);
    printf("%d\n",fd[1]);
    close(fd[0]);
    close(fd[1]);
    return 0;
}

