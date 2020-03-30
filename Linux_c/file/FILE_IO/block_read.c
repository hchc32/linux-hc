#include <stdio.h>
#include<unistd.h>
int main()
{
    //阻塞现象
    char buf[10];
    int n;
    n = read(STDIN_FILENO,buf,sizeof(buf)); //文件描述符宏定义，对应的文件/dev/tty
    if(n < 0)
    {
        printf("write error!\n");
    }
    write(STDOUT_FILENO,buf,sizeof(buf));
    return 0;
}

