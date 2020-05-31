#include<stdio.h>
#include<pthread.h>

//无头结点双向链表插入操作宏定义
#define  LL_ADD(list,pnew) {\
    pnew->perv = NULL; \
    pnew->next = list; \
    if(list != NULL) list->prev = pnew; \
    list = pnew; \
}

//删除操作
#define LL_DELETE(list,pdel) {\
    if(pdel->prev != NULL) pdel->prev->next = pdel->next; \
    if(pdel->next != NULL) pdel->next->prev = pdel->prev; \
    if(pdel == list) list = pdel->next; \
    pdel->next = pdel->prev = NULL; \
}

//执行队列
struct WORKER
{
    //线程ID
    pthread_t thread;
    
    struct WORKER *next;
    struct WORKER *prev;
};

//任务队列
struct JOB
{
    //任务函数
    void (*fun)(struct JOB *job);
    void *user_data;
    
    struct JOB *next;
    struct JOB *perv;
};

//管理组件
struct MANAGER
{
    struct WORKER *works;
    struct JOB *jobs;

    //条件变量
    pthread_cond_t jobs_cond;
    pthread_mutex_t jobs_mutex;
};
