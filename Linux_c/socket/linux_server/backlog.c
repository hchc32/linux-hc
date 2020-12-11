#include <stdio.h>             
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/mman.h>
#include<pthread.h>
#include<semaphore.h>
#include <netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<sys/epoll.h>
#include<dirent.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<errno.h>
#include<iostream>
#include<string>
#include<fstream>
#include<assert.h>
#include<signal.h>

static bool stop = false;

static void handle_term(int sig)
{
    stop = true;
}

int main(int argc , char *argv[])
{
    signal(SIGTERM,handle_term);
    if(argc < 3)
    {
        printf("usage: %s ip_address port backlog\n",basename(argv[0]));
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET,SOCK_STREAM,0);
    assert(sock >= 0);
    
    struct sockaddr_in address;
    bzero(&address , sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    //assert(ret != -1);

    ret = listen(sock,backlog);
    //assert(ret != -1);

    while( !stop )
    {
        sleep(1);
    }

    close(sock);
    return 0;
}
