#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>
#include<time.h>
#include<sys/times.h>

#define NUM 5
//链表结点类型
typedef struct things
{
    int count;
    //产品类型
    int which_produce;
    struct things* next;
}Node,*LinkList;


//定义互斥量
sem_t sem[NUM];
sem_t plate;


//定义头指针
LinkList head = NULL;

//生产者线程
void* produce(void* arg)
{
    int i = (int)arg;
    while(1)
    {
        //加锁失败        
        if(sem_wait(&plate) < 0) continue ;    
        //成功则生产东西
        else
        {
            Node* pnew = (Node*)malloc(sizeof(Node));
            //0~999
            pnew->count = rand()%1000;
            pnew->which_produce = i;
            pnew->next = head;
            head = pnew;
            printf("====== produce %3d : %lu, %d\n",i,pthread_self(),pnew->count);
            //盘子中的物品类型为ｉ的物品数
            sem_post(&sem[i]);
            //释放盘子
            sem_post(&plate);
            sleep(1);
        }
    }
}

//消费者线程
void* customer(void* arg)
{
    int i = (int)arg;
    while(1)
    {
       sleep(1);
        Node *pdel = NULL,*lpdel = NULL;
        if(sem_wait(&plate) < 0) continue;
        else
        {
            //判断链表是否为空
            if(head == NULL)
            {
                sem_post(&plate);
                continue;
            }
            if(sem_wait(&sem[i]) < 0)
            {
                sem_post(&plate);
                continue;
            }
            //链表不为空,删除结点
            if(head->which_produce == i)
            {
                pdel = head;
                head = head->next;
            }
            else
            {
                pdel = head;
                while(pdel->which_produce != i)
                {
                    lpdel = pdel;
                    pdel = pdel->next;
                }
                lpdel->next = pdel->next;
            }
            printf("------ customer %3d : %lu, %d\n",i,pthread_self(),pdel->count);
            free(pdel);
            sem_post(&plate);
        }
        sleep(rand() % 3);
    }
}

int main()
{
    //定义线程变量
    pthread_t pth_produce[NUM],pth_customer[NUM];
    sem_init(&plate,0,1);
    for(int i = 0; i < NUM; i++)
    {
        sem_init(&sem[i],0,0);
    }
    //创建生产者线程,创建消费者线程
    for(int i = 0; i < NUM; i++)
    {
        pthread_create(&pth_produce[i],NULL,produce,(void*)i);
        pthread_create(&pth_customer[i],NULL,customer,(void*)i);
    }
    //阻塞回收子线程
    for(int i = 0; i < NUM; i++)
    {
        pthread_join(pth_produce[i],NULL);
        pthread_join(pth_customer[i],NULL);
    }

    for(int i = 0; i < NUM; i++)
    {
        sem_destroy(&sem[i]);
    }
    sem_destroy(&plate);
    return 0;
}

