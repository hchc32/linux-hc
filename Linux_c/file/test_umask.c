#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
int main()
{
    umask(0);           //不屏蔽任何权限
    if(creat("example_681.test",S_IRWXU | S_IRWXG | S_IRWXO) < 0)
    {
        perror("creat");
        exit(1);
    }
    umask(S_IRWXO);
    if(creat("example_682.test",S_IRWXU | S_IRWXG | S_IRWXO) < 0)
    {
        perror("creat");
        exit(1);
    }
    return 0;
}
//S_IRWXU  用户所有权限 
//S_IRWXG  用户组所有权限
//S_IRWXO  其他用户所有权限
