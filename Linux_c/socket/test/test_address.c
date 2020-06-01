#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    char buffer[32];
    int ret = 0;
    int host = 0;
    int network = 0;
    unsigned int address = 0;
    struct in_addr in;
    in.s_addr = 0 ;

    printf("please input your ip address: ");
    fgets(buffer , 32 , stdin);
    buffer[32] = '\0';

    //inet_aton函数
    if((ret = inet_aton(buffer,&in)) == 0)
    {
        printf("inet_aton: \t invalid address\n");
    }
    else
    {
        printf("inet_aton:\t Ox%x\n",in.s_addr);
    }

    //inet_addr函数  255.255.255.255
    if((address = inet_addr(buffer)) == INADDR_NONE)
    {
        printf("inet_addr: \t invalid address\n");
    }
    else
    {
        printf("inet_addr:\t Ox%x\n",address);
    }

    //inet_network函数 同上
    if((address = inet_network(buffer)) == -1)
    {
        printf("inet_network: \t invalid address\n");
    }
    else
    {
        printf("inet_network:\t Ox%x\n",address);
    }

    //inet_ntoa函数
    if((inet_ntoa(in)) == NULL)
    {
        printf("inet_ntoa: \t invalid address\n");
    }
    else
    {
        printf("inet_ntoa:\t %s\n",inet_ntoa(in));
    }

    //inet_lnaof  ip地址中的主机地址
    //inet_netof  ip地址中的网络地址
    //host == 主机
    host = inet_lnaof(in);
    network = inet_netof(in);
    printf("inet_lnaof:\tOx%x\n",host);
    printf("inet_netof:\tOx%x\n",network);

    in = inet_makeaddr(network,host);
    printf("inet_makeaddr:\tOx%x\n",in.s_addr);


    return 0;
}

