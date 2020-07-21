#ifndef _USER_PASSWORD_H
#define _USER_PASSWORD_H
#include<mysql/mysql.h>
#include<stdio.h>

//用户信息
typedef struct zhuce
{
    char user_name[20];   //用户昵称
    char password[20];    //密码
    char birthday[10];    //生日
    char accounts[10];    //帐号
    int cs_flag;          //未知
}regist;

int JoinMysql(MYSQL *mysql,regist *info);
int get_userinfo(char *buf,int len);
void input_userinfo(int conn_fd,const char *string);

#endif
