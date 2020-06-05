#include <stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<string.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/stat.h>

#define BUF_SIZE 256
#define PROJ_ID 32
#define PATH_NAME "/tmp"
//消息队列中消息类型
#define SERVER_MSG 1
#define CLIENT_MSG 2

int main()
{
    //用户自定义消息缓冲区
    struct mymsgbuf
    {
        long msgtype;
        char ctrlstring[BUF_SIZE];
    }msgbuffer;
    //消息队列标识符
    int qid;
    //消息长度
    int msglen;
    //键值
    key_t msgkey;

    //获区键值
    if((msgkey = ftok(PATH_NAME,PROJ_ID)) == -1)
    {
        perror("ftok error!");
        exit(1);
    }

    //获取消息队列标识符
    if((qid = msgget(msgkey,IPC_CREAT | 0666)) == -1)
    {
        perror("msgget error!");
        exit(0);
    }

    //读写操作
    while(1)
    {
        printf("Server:");
        fgets(msgbuffer.ctrlstring , BUF_SIZE ,stdin);
        if(strncmp("exit",msgbuffer.ctrlstring,4) == 0)
        {
            msgctl(qid,IPC_RMID,NULL);
            break;
        }
        msgbuffer.ctrlstring[strlen(msgbuffer.ctrlstring)-1] = '\0';
        msgbuffer.msgtype = SERVER_MSG;
        if((msgsnd(qid , &msgbuffer ,strlen(msgbuffer.ctrlstring),0)) == -1)
        {
            perror("server msgsnd error!");
            exit(1);
        }

        if(msgrcv(qid,&msgbuffer,BUF_SIZE,CLIENT_MSG , 0) == -1)
        {
            perror("server msgrcv error!");
            exit(1);
        }
        printf("Client: %s\n",msgbuffer.ctrlstring);
    }
    return 0;
}

