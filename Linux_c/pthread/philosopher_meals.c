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
    while(1)
    {
        sleep(1);
        //拿到左手筷子
        pthread_mutex_lock(&mutex[left]);
        //尝试去抢右手的筷子
        //抢不到右手的筷子
        if(pthread_mutex_trylock(&mutex[right]) != 0)
        {
            pthread_mutex_unlock(&mutex[left]);
            continue;
        }
        //抢到了右手的筷子
        //printf("哲学家 %d 抢到了筷子　%d %d\n",i,left,right);
        for(int j = 0; j < 5; j++)
        {
            if(j == i)
            {
                printf("=====哲学家 %d　正在吃饭\n",i);
            }
            else
            {
                printf("-----哲学家 %d  正在think!\n",j);
            }
        }
        
        sleep(2);

        //吃完饭释放筷子
        pthread_mutex_unlock(&mutex[left]);
        pthread_mutex_unlock(&mutex[right]);
    }
    return NULL;
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

