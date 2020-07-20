#ifndef _MYSQL_H
#define _MYSQL_H
#include<mysql/mysql.h>

int connect_mysql(MYSQL *mysql);
int close_mysql(MYSQL *mysql);

#endif
