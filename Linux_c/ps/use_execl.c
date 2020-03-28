#include <stdio.h>
#include<unistd.h>

int main()
{
    //execlp("ls","ls","-l",NULL);
    execl ("/bin/ls","ls","-l","--color=auto",NULL);
    perror("exec err");
    printf("hello\n");
    return 0;
}
