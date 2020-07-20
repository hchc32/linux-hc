#include<stdio.h>
//登录信息
typedef struct users
{
    char username[20];     //管理员用户名
    char password[20];     //管理员密码
}User;

void 	user_register(void);							//管理员信息注册函数 
int 	user_pass(void);								//管理员登录函数 

//管理员信息存储绝对路径名
char u[]="/home/hc/桌面/2.txt";

