    sprintf(mysql_temp,"select *from 帐号密码 where 帐号='%s'",temp->friend_accounts);
    if(mysql_query(&mysql,mysql_temp) < 0)
    {   
        my_err("send",__LINE__);
    }
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    int friend_fd = atoi(row[0]);
    int flag = atoi(row[5]);
    //在线
    if(flag == 1)
    {
        strcpy(send_temp.strings,"你有一条验证消息!");
        //发送给friend验证消息
        if(send(friend_fd,&send_temp,sizeof(send_temp),0) < 0)
        {
            my_err("send",__LINE__);
        }
    }

