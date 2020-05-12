#include <stdio.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>

int number;
pthread_rwlock_t lock;
void* write_func(void *arg)
{
    //循环写
    while(1)
    {
        //加写锁
        pthread_rwlock_wrlock(&lock);
        number++;
        printf("write: %lu ,%d\n",pthread_self(),number);
        //解锁
        pthread_rwlock_unlock(&lock);
        usleep(500);
    }

    return NULL;
}

void* read_func(void *arg)
{
    while(1)
    {
        //加读锁
        pthread_rwlock_rdlock(&lock);
        printf("read: %lu ,%d\n",pthread_self(),number);
        pthread_rwlock_unlock(&lock);
        usleep(500);
    }
    return NULL;
}
int main(int argc,char *argv[])
{
    pthread_t p[8];
    //初始化读写锁
    pthread_rwlock_init(&lock,NULL);

    //创建３个写线程
    for(int i = 0; i < 3; i++)
    {
        pthread_create(&p[i],NULL,write_func,NULL);
    }
    //创建５个读线程
    for(int i = 3; i < 8; i++)
    {
        pthread_create(&p[i],NULL,read_func,NULL);
    }

    //阻塞回收子线程的PCB资源
    for(int i = 0; i < 8; i++)
    {
        pthread_join(p[i],NULL);
    }

    //释放读写锁资源
    pthread_rwlock_destroy(&lock);
    return 0;
}

