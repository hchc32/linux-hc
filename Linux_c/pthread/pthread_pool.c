#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>

//无头结点双向链表插入操作宏定义
#define  LL_ADD(list,pnew) {\
    pnew->prev = NULL; \
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
    
    struct MANAGER *pool;
    //终止标志
    int terminate;
    struct WORKER *next;
    struct WORKER *prev;
};

//任务队列
struct JOB
{
    //任务函数
    //void (*fun)(struct JOB *job);
    void (*fun)(void* arg);
    void *user_data;
    
    struct JOB *next;
    struct JOB *prev;
};

//管理组件
struct MANAGER
{
    struct WORKER *works;
    struct JOB *jobs;

    //条件变量
    pthread_cond_t jobs_cond;
    //互斥锁
    pthread_mutex_t jobs_mutex;
};

typedef struct MANAGER ThreadPool;

//DeBug_Test
void DeBug_Test(void *number)
{
    int num = (int)number;
    printf("%d\n",num);
}
//ThreadCallBackFun
 void * ThreadCallBackFun(void *arg)
{
    struct WORKER *worker = (struct WORKER*)arg;
    
    //两种状态:等待和工作
    while(1)
    {

        while(worker->pool->jobs == NULL)
        {
            if(worker->terminate) break;
            //条件变量阻塞等待
            pthread_cond_wait(&worker->pool->jobs_cond,&worker->pool->jobs_mutex);
        }
        if(worker->terminate)
        {
            pthread_mutex_unlock(&worker->pool->jobs_mutex);
            break;
        }
        struct JOB *job = worker->pool->jobs;
        //从任务队列中删除
        LL_DELETE(worker->pool->jobs,job);
        /*
        job->fun = DeBug_Test;        
        */
        job->fun(job->user_data);
        pthread_mutex_unlock(&worker->pool->jobs_mutex);
    }
    free(worker);
    pthread_exit(NULL);
}

//创建线程池接口
//numWorks --> 线程数目
int ThreadPoolCreat(ThreadPool *pool , int numWorks)
{
    if(numWorks < 1) numWorks = 1;
    if(pool == NULL) return -1;
    memset(pool,0,sizeof(ThreadPool));
    pthread_cond_t temp_cond = PTHREAD_COND_INITIALIZER;
    memcpy(&pool->jobs_cond , &temp_cond ,sizeof(pthread_cond_t));
    pthread_mutex_t temp_mutex = PTHREAD_MUTEX_INITIALIZER;
    memcpy(&pool->jobs_mutex , &temp_mutex ,sizeof(pthread_mutex_t));
    for(int i = 0; i < numWorks ; i++) 
    {
        struct WORKER *worker = (struct WORKER*)malloc(sizeof(struct WORKER));
        if(worker == NULL)
        {
            perror("malloc");
            return -1;
        }
        memset(worker, 0 ,sizeof(struct WORKER));
        worker->pool = pool;

        int ret = pthread_create(&worker->thread,NULL,ThreadCallBackFun,(void*)worker);
        if( ret )
        {
            perror("pthread_creat");
            free(worker);
            return -1;
        }
        
        //将创建的线程加入到执行队列中
        LL_ADD(pool->works,worker);
    }
    return 1;
}//将numWorks数目的线程初始化并加入到执行队列中

//销毁线程池
int ThreadPoolDestroy(ThreadPool *pool)
{
    struct WORKER *worker = NULL;
    for(worker = pool->works ; worker != NULL; worker = worker->next)
    {
        //表示终止状态
        worker->terminate = 1;
    }
    pthread_mutex_lock(&pool->jobs_mutex);
    pthread_cond_broadcast(&pool->jobs_cond);
    pthread_mutex_unlock(&pool->jobs_mutex);
}

//往线程池中抛任务
void ThreadPoolPush(ThreadPool *pool , struct JOB *job)
{
    pthread_mutex_lock(&pool->jobs_mutex);
    LL_ADD(pool->jobs,job);
    //唤醒wait
    pthread_cond_signal(&pool->jobs_cond);
    pthread_mutex_unlock(&pool->jobs_mutex);
}

int main()
{
    struct JOB jobs;
    ThreadPool pool;
    ThreadPoolCreat(&pool,11);
    for(int i = 0; i < 10; i++)
    {
        jobs.fun = DeBug_Test;
        jobs.user_data = (void*)i;
        ThreadPoolPush(&pool,&jobs);
    }
    ThreadPoolDestroy(&pool);
    return 0;
}

