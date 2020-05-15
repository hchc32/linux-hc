#include <stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

//5把互斥锁
pthread_mutex_t mutex[5];

void* meals(void* arg)
{
    //第一个哲学家
    int i = (int)arg;
    //左右筷子的编号 ---- 锁
    int left , right;
    //使一个哲学家的行为与其他人不同
    if(i == 4)
    {
        left = 0;right = i;
    }
    else
    {
        left = i + 1;
        right = i;
    }
    
    //争夺筷子　－－－　锁
    while(pthread_mutex_lock(&mutex[i]) != 0)
    {
        
    }
}

int main()
{
    pthread_t pth[5];
    
    //初始化互斥锁
    //创建５个线程
    for(int i = 0 ; i < 5 ; i++)
    {
        pthread_mutex_init(&mutex[i],NULL);
        pthread_create(&pth[i],NULL,meals,(void*)i);
    }

    //回收5个线程
    for(int i = 0; i < 5; i++)
    {
        pthread_join(pth[i],NULL);
    }

    //销毁互斥锁
    for(int i = 0; i < 5 ; i++)
    {
        pthread_mutex_destroy(&mutex[i]);
    }
    return 0;
}

