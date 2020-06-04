#include <stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<string.h>

#define BUF_SIZE 256
#define PROJ_ID 32
#define PATH_NAME "."


int main()
{
    //用户自定义消息缓冲
    struct mymsgbuf
    {
        //消息类型
        long msgtype;
        //消息
        char ctrlstring[BUF_SIZE];
    }msgbuffer;
    //消息队列标识符
    int qid;
    //消息队列的大小
    int msglen;
    //消息队列的键值
    key_t msgkey;
    
    //获取一个键值
    if((msgkey = ftok(PATH_NAME,PROJ_ID)) == -1)
    {
        perror("ftok error!");
        exit(0);
    }

    //创建一个消息队列
    if((qid = msgget(msgkey,IPC_CREAT|0666)) == -1)
    {
        perror("msgget error!");
        exit(0);
    }

    //填充消息结构，发送至消息队列
    msgbuffer.msgtype = 3;
    strcpy(msgbuffer.ctrlstring , "Hello,message queue");
    msglen = sizeof(struct mymsgbuf)-4;
    if(msgsnd(qid,&msgbuffer,msglen,0) == -1)
    {
        perror("msgget error!\n");
        exit(1);
    }
    return 0;
}

