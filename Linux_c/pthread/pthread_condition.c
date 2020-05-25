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

//线程同步 - 互斥锁
pthread_mutex_t mutex;

//阻塞线程
pthread_cond_t cond;

void* producer(void* arg)
{
    while(1)
    {
        //创建一个链表的结点
        Node* pnew = (Node*)malloc(sizeof(Node));
        
        //数据域
        pnew->data = rand() % 1000; //0 ~ 999
        
        //使用互斥锁保护共享数据
        pthread_mutex_lock(&mutex);
        pnew->next = head;
        head = pnew;
        printf("====== produce: %lu, %d\n",pthread_self(),pnew->data);
        pthread_mutex_unlock(&mutex);
        
        //通知阻塞的消费者线程,解除阻塞
        pthread_cond_signal(&cond);

        //生产一个产品就休息３秒以内的时间
        sleep(rand() % 3);
    }
}

void* customer(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        //判断链表是否为空
        if(head == NULL)
        {
            //线程阻塞
            //该函数会对互斥锁解锁
            pthread_cond_wait(&cond,&mutex);
            //解除阻塞之后，对互斥锁进行加锁操作
        }
        //链表不为空,删除一个结点
        Node* pdel = head;
        head = head->next;
        printf("------ customer: %lu, %d\n",pthread_self(),pdel->data);
        free(pdel);
        pthread_mutex_unlock(&mutex);
    }
}


int main()
{
    pthread_t p1,p2;

    //初始化
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);

    //创建生产者线程
    pthread_create(&p1,NULL,producer,NULL);
    //创建消费者线程
    pthread_create(&p2,NULL,customer,NULL);
    

    //阻塞回收子进程
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    //销毁锁和条件变量 
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}

