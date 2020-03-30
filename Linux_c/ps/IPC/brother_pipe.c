//兄弟间通信
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int main()
{
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe");
        exit(1);
    }

    int i = 0;
    for(i = 0; i < 2; i++)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            break;
        }
        else if(pid == -1)
        {
            perror("fork");
            exit(1);
        }
    }
    //子进程1
    //ps aux
    if( i == 0 )
    {
        sleep(2);// 可以说明管道是阻塞
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ps","ps","aux",NULL);
        perror("execlp");
        exit(1);
    }
    //子进程2
    //grep bash
    else if(i == 1)
    {
        close(fd[1]);
        dup2(fd[0],0);
        execlp("grep","grep","--color=auto","bash",NULL);
        perror("execlp");
        exit(1);
    }
    //父进程
    else if(i == 2)
    {
        close(fd[0]);
        close(fd[1]);
        //回收子进程
        pid_t wpid;
        while((wpid = waitpid(-1,NULL,WNOHANG)) != -1)
        {
            if(wpid == 0)
            {
                continue;
            }
            printf("child died pid = %d\n",wpid);
        }
    }
    printf("pipe[0] = %d\n",fd[0]);
    printf("pipe[1] = %d\n",fd[1]);
    close(fd[0]);
    close(fd[1]);
    return 0;
}

