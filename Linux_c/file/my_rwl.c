//如果用creat函数创建文件夹，会出现Bad file descriptor 错误，这是因为creat创建的文件只有write的权限
//相当于open中第二个参数中有O_WRONLY


#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<error.h>
#include<stdlib.h>
#include<string.h>

//自定义的错误处理函数
void my_err(const char *err_string,int line)
{
    fprintf(stderr,"line: %d",line);
    perror(err_string);
    exit(1);
}
//自定义的读数据函数
int my_read(int fd)
{
    int len;
    int ret;
    int i;
    char read_buf[64];
//__LINE__这是预编译的内置宏
    if(lseek(fd,0,SEEK_END) == -1)
    {
        my_err("lseek",__LINE__);
    }
    if((len = lseek(fd,0,SEEK_CUR)) == -1)
    {
        my_err("lseek",__LINE__);
    }
    if((lseek(fd,0,SEEK_SET)) == -1)
    {
        my_err("lseek",__LINE__);
    }
    printf("len: %d\n", len);

    if((ret = read(fd,read_buf, len)) < 0)
    {
        my_err("read",__LINE__);
    }

    for(i=0;i<len;i++)
    {
        printf("%c",read_buf[i]);
    }
    printf("\n");
    return ret;
}

int main()
{
    int fd;
    char write_buf[32] = "Hello World!";
//在当前目录下创建文件  
    //if((fd = creat("example_63.c",S_IRWXU)) == -1)
   // {
    if((fd = open("example_63.c", O_RDWR | O_CREAT | O_TRUNC,S_IRWXU)) == -1){
        my_err("open",__LINE__);
    }
    else
    {
        printf("Creat file success\n");
    }
    
    if((write(fd,write_buf,strlen(write_buf))) != strlen(write_buf))
    {
        my_err("write",__LINE__);
    }
    printf("/*--------------------*/\n");
    if(lseek(fd ,10, SEEK_END) == -1)
    {
        my_err("lseek",__LINE__);
    }
    if(write(fd,write_buf,strlen(write_buf)) != strlen(write_buf))
    {
        my_err("write",__LINE__);
    }
    my_read(fd);
    close(fd);
    return 0;
}

