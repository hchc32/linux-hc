#include <stdio.h>
#include<unistd.h>
#include<errno.h>
#include<stdlib.h>
int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe");
        exit(1);
    }
    //测试管道的大小
    long size = fpathconf(fd[0],_PC_PIPE_BUF);
    printf("pipe buf = %ldK\n",size/1024);

    printf("pipe[0] = %d\n",fd[0]);
    printf("pipe[1] = %d\n",fd[1]);
    close(fd[0]);
    close(fd[1]);
    return 0;
}


