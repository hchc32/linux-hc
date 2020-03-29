#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
    int fd;
    fd = open("test.txt",O_CREAT|O_RDWR,0644);
    pid_t pid = fork();
    if(pid == 0)
    {
        write (fd,"hello\n",6);
    }
    else if(pid > 0)
    {
        char buf[20];
        memset(buf,'\0',sizeof(buf));
        sleep(1);
        read(fd,buf,sizeof(buf));
        printf("%s\n",buf);
    }
    return 0;
}

