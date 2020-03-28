//父子进程满足读时共享，写时复制
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>

int var = 100;

int main()
{
    pid_t pid = fork();
    if(pid == 0)
    {
        //son
        printf("var = %d,child,pid = %d,ppid = %d\n",var,getpid(),getppid());
        var = 1001;
        printf("var = %d,child,pid = %d,ppid = %d\n",var,getpid(),getppid());
        sleep(3);
        printf("var = %d,child,pid = %d,ppid = %d\n",var,getpid(),getppid());
    }
    else if(pid > 0)
    {
        //parent
        sleep(1); //保障子进程能够修改var的值成功
        printf("var = %d,parent,pid = %d,ppid = %d\n",var,getpid(),getppid());
        var = 2000;
        printf("var = %d,parent,pid = %d,ppid = %d\n",var,getpid(),getppid());
    }
    return 0;
}
