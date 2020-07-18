#include <stdio.h>
#include<mysql/mysql.h>
#include<string.h>


int main()
{
    MYSQL mysql;

    //初始化一个句柄
    mysql_init(&mysql);

    //初始化数据库
    mysql_library_init(0,NULL,NULL);

    //连接数据库
    mysql_real_connect(&mysql,"127.0.0.1","root","huchen12345677","try",0,NULL,0);

    //设置中文字符集
    mysql_set_character_set(&mysql,"utf8");

    //获得表中所有数据
    mysql_query(&mysql,"select *from 好友关系");
    MYSQL_RES *result = mysql_store_result(&mysql);

    //获取表的列数
    unsigned int num_fields = mysql_num_fields(result);

    //读取每一行数据
    MYSQL_ROW row ;
    char buf[20];
    while((row = mysql_fetch_row(result)))
    {
    strcpy(buf,row[0]);
        /*
        for(int i = 0; i < num_fields ; i++)
        {
            printf("%-20s",row[i]);
        }
        */
        printf("\n");

    }
    printf("%s",buf);
    //关闭数据库
    mysql_close(&mysql);
    
    //关闭数据库连接
    mysql_library_end();
    return 0;
}

