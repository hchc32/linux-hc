#include <stdio.h>
#include<time.h>
#include<sys/stat.h>
#include<errno.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
    struct stat buf;

    //检查参数个数
    if(argc != 2)
    {
        printf("Filename\n");
        exit(0);
    }
    
    //获取文件属性
    if(stat(argv[1],&buf) == -1)
    {
        perror("stat:");
        exit(1);
    }

    //打印出文件属性
    printf("device is: %d\n",buf.st_dev);       //文件的设备编号
    printf("inode is:%d\n",buf.st_ino);         //文件的i-node结点编号
    printf("mode is:%o\n",buf.st_mode);         //文件的类型和存取权限
    printf("number of hard links is::%d\n",buf.st_nlink);     //连接到文件的硬链接数目
    printf("user ID of owner is:%d\n",buf.st_uid);            //文件所有者的ID
    printf("group ID of owner is: %d\n",buf.st_gid);          //文件所有者组的ID
    printf("device type (if inode device) is:%d\n",buf.st_rdev); //若此文件为设备文件，则为其设备编号
    printf("total size ,in bytes is:%d\n",buf.st_size);         //文件的大小
    printf("time of last access is:%s",ctime(&buf.st_atime));   //文件最近一次被访问的时间
    printf("time of last modification is:%s",ctime(&buf.st_mtime));  //文件最后一次被修改的时间
    printf("time of last change is:%s",ctime(&buf.st_ctime));       //文件最近一次被更改的时间
    return 0;
}

