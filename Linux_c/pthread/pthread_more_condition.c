#include <stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

//链表结点类型
typedef struct things
{
    int count;
    struct things* next;
}Node,*LinkList;

//定义锁和条件变量
pthread_mutex_t mutex;
pthread_cond_t cond;

//定义头结点
LinkList head = NULL;

//生产者线程
void* produce(void* arg)
{
    int i = (int)arg;
    while(1)
    {
        //尝试加锁失败        
        if(pthread_mutex_trylock(&mutex) != 0)    
        {
            continue;
        }
        //成功则生产东西
        else
        {
            Node* pnew = (Node*)malloc(sizeof(Node));
            //0~999
            pnew->count = rand()%1000;
            pnew->next = head;
            head = pnew;
            printf("====== produce %3d : %lu, %d\n",i,pthread_self(),pnew->count);
            pthread_mutex_unlock(&mutex);
            //通知阻塞的消费者线程，解除阻塞
            pthread_cond_signal(&cond);
            sleep(rand() % 3);
        }
    }
    return NULL;
}

//消费者线程
void* customer(void* arg)
{
    int i = (int)arg;
    while(1)
    {
        //尝试加锁失败
        if(pthread_mutex_trylock(&mutex) != 0)
        {
            continue;
        }
        //加锁成功
        else
        {
            //判断链表是否为空
            if(head == NULL)
            {
                pthread_cond_wait(&cond,&mutex);    
            }
            //链表不为空,删除结点
            Node* pdel = head;
            head = head->next;
            printf("------ customer %3d : %lu, %d\n",i,pthread_self(),pdel->count);
            free(pdel);
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

int main()
{
    //定义线程变量
    pthread_t pth_produce[3],pth_customer[3];

    //初始化条件变量,初始化锁
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);
    //创建生产者线程,创建消费者线程
    for(int i = 0; i < 3; i++)
    {
        pthread_create(&pth_produce[i],NULL,produce,(void*)i);
        pthread_create(&pth_customer[i],NULL,customer,(void*)i);
    }
    //阻塞回收子线程
    for(int i = 0; i < 3; i++)
    {
        pthread_join(pth_produce[i],NULL);
        pthread_join(pth_customer[i],NULL);
    }
    //销毁条件变量
    pthread_cond_destroy(&cond);
    //销毁锁
    pthread_mutex_destroy(&mutex);
    return 0;
}

