#ifndef _USER_PASSWORD_H
#define _USER_PASSWORD_H
#include<mysql/mysql.h>
#include<stdio.h>

#define REGIST     1
#define LOGIN      2
#define FINDPASS   3
#define EXIT       4
#define ADDFRIEND  5
#define FRIENDLIST 6

//消息类型
#define ADDF       20
//用户信息
//注册包
typedef struct zhuce
{
    char accounts[10];    //帐号
    char user_name[20];   //用户昵称
    char password[20];    //密码
    char phone_num[11];   //电话号码
}Regist;

//登录包
typedef struct denglu
{
    char accounts[10];   //帐号
    char password[20];   //密码
}Log;
//找回密码包
typedef struct zhaohui  
{
    char accounts[10];  //帐号
    char phone_num[11]; //手机号码
}FindPass;

//添加好友包
typedef struct addf
{
    char my_accounts[10];
    char friend_accounts[10];
}Addfriend;

//通用数据包
typedef struct data_packet
{
    int type;           //消息类型
    char strings[1024]; //消息
}Data;

int JoinMysql(MYSQL *mysql,Regist *info);
int get_userinfo(char *buf,int len);
char* login_userinfo(int conn_fd);
int find_userinfo(int conn_fd);

#endif
