#include <stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<semaphore.h>

sem_t produce_sem;
sem_t custom_sem;

typedef struct thing
{
    int data;
    struct thing* next;
}Node,*LinkList;

Node* head = NULL;

void* produce(void* arg)
{
    while(1)
    {
        sem_wait(&produce_sem);
        Node* pnew = (Node*)malloc(sizeof(Node));
        pnew->data = rand()%1000; //0~999
        pnew->next = head;
        head = pnew;
        printf("====== 生产者:%lu, %d\n",pthread_self(),pnew->data);
        sem_post(&custom_sem);
        sleep(rand()%3);
    }
    return NULL;
}

void* custom(void* arg)
{
    while(1)
    {
        sem_wait(&custom_sem);
        Node* pdel = head;
        head = head->next;
        printf("------ 消费者:%lu, %d\n",pthread_self(),pdel->data);
        free(pdel);
        sem_post(&produce_sem);
        sleep(rand()%3);
    }
    return NULL;
}
int main()    
{
    pthread_t p1,p2;
    sem_init(&produce_sem,0,2);
    sem_init(&custom_sem,0,0);
    pthread_create(&p1,NULL,produce,NULL);
    pthread_create(&p2,NULL,custom,NULL);

    pthread_join(p1,NULL);
    pthread_join(p2,NULL);

    sem_destroy(&produce_sem);
    sem_destroy(&custom_sem);

    return 0;
}

