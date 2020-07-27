#ifndef _USER_PASSWORD_H
#define _USER_PASSWORD_H
#include<mysql/mysql.h>
#include<stdio.h>

//用户信息
typedef struct zhuce
{
    char accounts[10];    //帐号
    char user_name[20];   //用户昵称
    char password[20];    //密码
    char phone_num[11];    //电话号码
    int cs_flag;          //在线状态
}regist;

int JoinMysql(MYSQL *mysql,regist *info);
int get_userinfo(char *buf,int len);
int input_userinfo(int conn_fd,const char *string);

#endif
