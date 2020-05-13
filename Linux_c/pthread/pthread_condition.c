#include <stdio.h>
#include<pthread.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
//链表的数据结点
typedef struct things
{
    int data;
    struct things* next;
}Node,*LinkList;

//指向链表头部的指针
Node* head = NULL;

void* producer(void* arg)
{
    while(1)
    {
        //创建一个链表的结点
        Node* pnew = (Node*)malloc(sizeof(Node));
        pnew->data = rand() % 1000; //0 ~ 999
        pnew->next = head;
        head = pnew;
        //生产一个产品就休息３秒以内的时间
        sleep(rand() % 3);
    }
}

void* customer(void* arg)
{

}


int main()
{
    pthread_t p1,p2;
    //创建生产者线程
    pthread_create(&p1,NULL,producer,NULL);
    //创建消费者线程
    pthread_create(&p1,NULL,customer,NULL);
    

    //阻塞回收子进程
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    return 0;
}

