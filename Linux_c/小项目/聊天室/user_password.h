#ifndef _USER_PASSWORD_H
#define _USER_PASSWORD_H
#include<mysql/mysql.h>
#include<stdio.h>

#define INVALID_USERINFO 'n' //用户信息无效
#define VALID_USERINFO   'y' //用户信息有效
#define EXIT        -1
#define REGIST      1
#define LOGIN       2
#define FINDPASS    3
#define ADDFRIEND   4
#define DELFRIEND   5
#define LOOKFRIEND  6
#define FRIENDLIST  7
#define DATABOX     8
#define FRIENDAPPLY 9
#define PRIVCHAT    10
#define SENDFILE    18
#define RECVFILE    19
#define GROUPCHAT   31 

//用户消息类型
#define ADDF       20
#define ADDFRETURN 21
#define ADDG       22
#define ADDGRETURN 23
#define MESSAGE    30
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
    char user_name[20];  //用户昵称
}Log;
//找回密码包
typedef struct zhaohui  
{
    char accounts[10];  //帐号
    char phone_num[11]; //手机号码
}FindPass;

//添加/删除好友包,用于添加好友请求，处理好友请求，
typedef struct addf
{
    int  f_g_flag;
    char flag;                 //加好友同意(y)与否(n)的标志, 
    char my_accounts[10];
    char friend_accounts[10];
}Addfriend;

//获取好友列表包
typedef struct haoyouxinxi
{
    char accounts[10];    //帐号
    char user_name[20];   //用户昵称
    int flag;            //在线状态
}Friend_data;
typedef struct haoyou
{
    Friend_data data;
    struct haoyou *prev;
    struct haoyou *next;
}Friend;

//消息盒子
typedef struct s_hezi 
{
    char s_accounts[10];
    char r_accounts[10];
    int data_type;
    char data_string[100];
}s_data;
typedef struct hezi
{
    s_data data;
    struct hezi *prev;
    struct hezi *next;
}Box;

typedef struct wenjian
{
    char s_accounts[10];
    char r_accounts[10];
    char filename[30];
}File_Rs;

typedef struct recvfile
{
    int fd;
    char filename[20];
}File_Recv;

//通用数据包
typedef struct data_packet
{
    int type;           //消息类型
    char strings[1024]; //消息
}Data;

int JoinMysql(MYSQL *mysql,Regist *info);
int get_userinfo(char *buf,int len);
Log login_userinfo(int conn_fd);
int find_userinfo(int conn_fd);
int getch(void);
#endif
