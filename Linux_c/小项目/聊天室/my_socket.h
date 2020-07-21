#ifndef _MY_SOCKET_H
#define _MY_SOCKET_H

#define INVALID_USERINFO 'n' //用户信息有效
#define VALID_USERINFO   'y' //用户信息无效
#define SERV_PORT 4507
#define LISTEN_NUM 2000
#define BUFSIZE 1024
int get_lfd(void);
int my_recv(int conn_fd,char *data_buf,int len);
int my_send(int conn_fd,char *data_buf,int len);

#endif
