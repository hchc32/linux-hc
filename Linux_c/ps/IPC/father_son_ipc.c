#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
int main()
{
    int fd[2];
    //创建一块缓冲区————管道
    int temp = pipe(fd);
    if(temp == -1)
    {
        perror("pipe error !\n");
    }
    pid_t pid = fork();
    if(pid == 0)//子进程
    {
        //读管道操作,关闭写端
        close(fd[1]);
        //数据重定向，将STDIN_FILENO-->fd[0]
        dup2(fd[0],0);
        //执行grep、
        execlp("grep","grep","--color=auto","bash",NULL);
        perror("execlp");
        exit(1);
    }
    else if(pid > 0)//父进程
    {
        //写管道操作，关闭读端fd[0]
        close(fd[0]);
        //数据重定向，将STDOUT_FILENO-->fd[1]
        dup2(fd[1],1);
        //执行ps -aux
        execlp("ps","ps","aux",NULL);
        perror("execlp");
        exit(1);
    }
    return 0;
}

