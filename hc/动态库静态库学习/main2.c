//静态库
//1.创建两个.c文件
//2.生成可重定位的.o文件 gcc -c addvec.c multvec.c
//3.创建一个.h文件,声明两个.c文件里面的函数
//4.ar rcs libvector.a addvec.o multvec.o 用来创建静态库
//5.gcc -static -o main2 main2.o       ./libvector.a 用来使用静态库 
//main2为可执行文件名
//-static用来告诉编译器，应该构建一个完全链接的可执行文件
//查看静态库中的目录列表: ar -tv libvector.a 
//                          -t列出静态库信息
//                          -v列出详细信息
//


#include <stdio.h>
#include"vector.h"



int x[2] = {1,2};
int y[2] = {3,4};
int z[2];

int main()
{
    addvec(x,y,z,2);
    printf("z=[%d %d]\n",z[0],z[1]);
    return 0;
}

