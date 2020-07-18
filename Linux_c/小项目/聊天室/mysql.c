#include<stdio.h>
#include<mysql/mysql.h>
#include"my_error.c"

/*
 *函数名:connect_mysql
 *描述  :用于连接数据库
 *参数  :mysql -- mysql句柄 
 *返回值:成功返回0,失败返回-1
*/
int connect_mysql(MYSQL *mysql)
{
    //初始化一个句柄
    if(mysql_init(mysql) == NULL)
    {
        my_err("mysql_init",__LINE__);
        return -1;
    }
        //初始化数据库
    if(mysql_library_init(0,NULL,NULL) != 0)
    {
        my_err("mysql_library_init",__LINE__);
        return -1;
    }

    //连接数据库
    if(mysql_real_connect(mysql,"127.0.0.1","root","huchen12345677","try",0,NULL,0) == NULL)
    {
        my_err("mysql_real_connect",__LINE__);
        return -1;
    }

    //设置中文字符集
    if(mysql_set_character_set(mysql,"utf8") < 0)
    {
        my_err("mysql_set_character_set",__LINE__);
        return -1;
    }
    return 0;

}

/*
 *函数名:close_mysql
 *描述:关闭数据库及连接
 *参数:mysql句柄
 *返回值:成功关闭返回0,否则返回-1
*/ 
int close_mysql(MYSQL *mysql)
{
    mysql_close(mysql);
    mysql_library_end();
    return 0;
}

/*
 *函数名:get_rows
 *描述  :获取数据库表的行数
 *参数  :mysql_strings -- mysql语句(不加分号)
 *返回值:成功返回行数,失败返-1
int get_rows(const char *mysql_strings)
{
    
}
*/








