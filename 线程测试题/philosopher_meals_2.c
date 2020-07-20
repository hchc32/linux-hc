#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
//定义5把互斥锁
pthread_mutex_t mutex[5];


void* meals(void *arg)
{
    int i = (int)arg;
    //序号为奇数，拿起左手的筷子
    while(1)
    {
        /*
        for(int i = 0; i < 5; i++)
        {
            printf("philosopher %d is thinking!!!\n",i);
        }
        */
        sleep(1);
        if( i%2!=0 )
        {
            pthread_mutex_lock(&mutex[(i+1)%5]);
            if(pthread_mutex_trylock(&mutex[i]) == 0)
            {
                printf("philosopher %d is eating!!!\n",i);
                pthread_mutex_unlock(&mutex[i]);
            }
/*

            {
                printf("pthilosopher %d is thinking!!!\n",i);
            }*/
            pthread_mutex_unlock(&mutex[(i+1)%5]);
        }
        else
        {
            pthread_mutex_lock(&mutex[i]);
            if(pthread_mutex_trylock(&mutex[(i+1)%5]) == 0)
            {
                printf("philosopher %d is eating!!!\n",i);
                pthread_mutex_unlock(&mutex[(i+1)%5]);
            }
            /*
            else
            {
                printf("pthilosopher %d is thinking!!!\n",i);
            }
            */
            pthread_mutex_unlock(&mutex[i]);
            sleep(2);
        }
        printf("=======================================\n");
    }   
}

int main()
{
    pthread_t thr[5];
    for(int i = 0; i < 5; i++)
    {
        pthread_mutex_init(&mutex[i],NULL);
        pthread_create(&thr[i],NULL,meals,(void*)i);
    }

    for(int i = 0 ; i < 5 ; i++)
    {
        pthread_join(thr[i],NULL);
        pthread_mutex_destroy(&mutex[i]);
    }
    return 0;
}

