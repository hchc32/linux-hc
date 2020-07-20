#include <stdio.h>
#include<mysql/mysql.h>
#include<string.h>
#include"mysql.h"
#include"user_password.h"

int main()
{
    MYSQL mysql;
    connect_mysql(&mysql);
    get_user_pass(&mysql);
    close_mysql(&mysql);
    return 0;
}
