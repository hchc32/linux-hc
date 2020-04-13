#include <stdio.h>
#include<unistd.h>
#include<fcntl.h>
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
    int flags;
    //获取文件权限
    flags = fcntl(fd[0],F_GETFL);
    flags |= O_NONBLOCK;
    //设置非阻塞
    fcntl(fd[0],F_SETFL,flags);
    pid_t pid = fork();
    if(pid > 0) //父进程
    {
    //    sleep(1);
        //写
        sleep(1);
        close(fd[0]);
        dup2(fd[1],1);
        execlp("ps","ps","ajx",NULL);
        perror("execlp");
        exit(1);
    }
    else if(pid == 0)//子进程
    {
        //读
        close(fd[1]);
        dup2(fd[0],0);
        execlp("grep","grep","--color=auto","bash",NULL);
        perror("execlp");
        exit(1);
    }
    return 0;
}

