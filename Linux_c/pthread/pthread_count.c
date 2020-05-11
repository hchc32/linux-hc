#include <stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

#define MAX 10000

int number;
//创建一把互斥锁
pthread_mutex_t mutex;

//线程处理函数
void* funA_num(void* arg)
{
    for(int i = 0; i < MAX; i++)
    {
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur++;
        number = cur;
        printf("Thread A , tid = %lu,number = %d\n",pthread_self(),number);
        pthread_mutex_unlock(&mutex);
        //微秒
        usleep(10);
    }
    return NULL;
}

void* funB_num(void* arg)
{
    for(int i = 0; i < MAX; i++)
    {
        //访问全局变量之前加锁
        //如果mutex被锁上了,代码阻塞在当前位置
        pthread_mutex_lock(&mutex);

        int cur = number;
        cur++;
        number = cur;
        printf("Thread B , tid = %lu,number = %d\n",pthread_self(),number);
        //解锁
        pthread_mutex_unlock(&mutex);
        usleep(10);
    }
    return NULL;
}

int main(int argc,char* argv[])
{
    pthread_t p1,p2;
    //初始化互斥锁
    pthread_mutex_init(&mutex,NULL);

    //创建两个子进程
    pthread_create(&p1,NULL,funA_num,NULL);
    pthread_create(&p2,NULL,funB_num,NULL);

    //回收线程的pcb
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    
    //释放互斥锁资源
    pthread_mutex_destroy(&mutex);
    return 0;
}

