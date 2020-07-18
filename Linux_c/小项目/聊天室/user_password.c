#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mysql/mysql.h>
#include"mysql.c"


/*
 *函数名:get_user_pass
 *描述  :账户注册
 *参数  :对应的数据库
 *返回值:void
*/
void get_user_pass(MYSQL *mysql)
{
    char user[10];
    char user_name[30];
    char password[30];
    char birthday[10];
    char temp[50];

    memset(user,'\0',sizeof(user));
    memset(user_name,'\0',sizeof(user_name));
    printf("请输入你要注册的帐号昵称:\n");
    scanf("%s",user_name);

    printf("请输入你的密码(不小于7位字符):\n");
    scanf("%s",password);
    
    printf("密保问题:\n");
    printf("你的生日是多少?\n:");
    scanf("%s",birthday);

    //查询帐号密码表中最后一个帐号
    //打开数据库
    if(connect_mysql(mysql) < 0)
    {
        my_err("connect_mysql",__LINE__);
        return ;
    }
    //获得表中所有数据
    mysql_query(mysql,"select *from 好友关系");        
    MYSQL_RES *result = mysql_store_result(mysql);

    //获取表的列数
    unsigned int num_fields = mysql_num_fields(result);

    //读取每一行数据
    MYSQL_ROW row ;
    while((row = mysql_fetch_row(result)))
    {
        strcpy(user,row[0]);
    }
    int change = atoi(user);
    change++ ;
    sprintf(user,"%d",change);

    //将帐号和密码存入表中
    sprintf(temp,"insert into 帐号密码表 values(%s,%s,%s,%s)",user,user_name,password,birthday);
    mysql_query(mysql,temp);

    //关闭数据库
    close_mysql(mysql);
    printf("你的帐号为:%s\n",user);
    printf("请牢记你的帐号和密码.\n");
    printf("注册成功!\n");
}
