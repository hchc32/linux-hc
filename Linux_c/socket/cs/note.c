#include <stdio.h>
#include<linux/socket.h>

/*
 *地址类型:AF_INET  Ipv4 tcp/ip协议
 *         AF_INET6 Ipv6 tcp/ip协议
 *         AF_UNIX  只能在本机内进行通信
*/

struct sockaddr
{
    unsigned short sa_family;        /*地址类型　AF_xxx*/
    char           sa_data[14];      /*14字节的协议地址*/
};


struct in_addr
{
    unsigned long s_addr;            /*32位的IP地址*/
};


struct sockaddr_in
{
    unsigned short       sin_family; /*地址类型*/
    unsigned short int   sin_port;   /*端口号*/
    struct in_addr       sin_addr;   /*IP地址*/
    unsigned char        sin_zero[8];/*填充字节,一般赋值为0*/
};
