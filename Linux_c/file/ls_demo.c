#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>
#include<fcntl.h>
int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("./a.out  filename\n");
        return -1;
    }
    struct stat file;
    stat(argv[1],&file);
    char str_mode[11];
    memset(str_mode,'-',sizeof(str_mode));
    //文件的属性
    if(S_ISLNK(file.st_mode)) str_mode[0] = 'l';
    if(S_ISREG(file.st_mode)) str_mode[0] = '-';
    if(S_ISDIR(file.st_mode)) str_mode[0] = 'd';
    if(S_ISCHR(file.st_mode)) str_mode[0] = 'c';
    if(S_ISBLK(file.st_mode)) str_mode[0] = 'b';
    if(S_ISFIFO(file.st_mode)) str_mode[0] = 'p';
    if(S_ISSOCK(file.st_mode)) str_mode[0] = 's';

    //文件的权限
    //用户
    if(file.st_mode & S_IRUSR) str_mode[1] = 'r';
    if(file.st_mode & S_IWUSR) str_mode[2] = 'w';
    if(file.st_mode & S_IXUSR) str_mode[3] = 'x';
    //用户组
    if(file.st_mode & S_IRGRP) str_mode[4] = 'r';
    if(file.st_mode & S_IWGRP) str_mode[5] = 'w';
    if(file.st_mode & S_IXGRP) str_mode[6] = 'x';
    //其他人
    if(file.st_mode & S_IROTH) str_mode[7] = 'r';
    if(file.st_mode & S_IWOTH) str_mode[8] = 'w';
    if(file.st_mode & S_IXOTH) str_mode[9] = 'x';
    str_mode[10] = '\0';
    //时间处理
    struct tm *filetm = localtime(&file.st_ctim.tv_sec);
    char timebuf[20] = {0};
    sprintf(timebuf,"%d月   %d %02d:%02d",filetm->tm_mon+1 ,filetm->tm_mday,filetm->tm_hour,filetm->tm_min);

    printf("%s %d %s %s %ld %s %s\n",str_mode,file.st_nlink,getpwuid(file.st_uid)->pw_name,getgrgid(file.st_gid)->gr_name
           ,file.st_size,timebuf,argv[1]);

    return 0;
}

