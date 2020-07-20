#include <stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
    char **arg;
    arg[0] = "ls";
    arg[1] = NULL;
    int fd;
    char *file = "1.txt";
    fd = open(file,O_RDWR | O_CREAT | O_TRUNC,0644);
    dup2(fd,1);
    execvp("ls",arg);
    

    return 0;
}

