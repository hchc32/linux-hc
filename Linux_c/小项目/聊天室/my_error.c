#include<stdio.h>                           
#include<stdlib.h>
/*自定义错误处理函数*/
void my_err(const char* err_string,int line)
{
    fprintf(stderr,"%d",line);
    perror(err_string);
    exit(1);
}

