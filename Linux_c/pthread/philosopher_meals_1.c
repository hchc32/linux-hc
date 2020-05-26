#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>
#include<stdlib.h>

#define N 5
//筷子状态为1 -- 空闲
//          0 -- 拿起
int state[N] = {1,1,1,1,1};
pthread_mutex_t mutex;

//拿起筷子
void take_chopsticks(int num)
{
    //如果左右筷子都空闲，则拿起
    if(state[num] == 1 && state[(num+1)%N] == 1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        state[num] = 0;
        state[(num+1)%N] = 0;
        //解锁
        pthread_mutex_unlock(&mutex);
    }
}

//放下筷子
void put_chopsticks(int num)
{
    pthread_mutex_lock(&mutex);
    state[num] = 1;
    state[(num+1)%N] = 1;
    pthread_mutex_unlock(&mutex);
}

//线程执行函数
void* philosopher(void* arg)
{
    //线程编号强转
    int i = (int)arg;
    //改变随机数的种子
    srand(time(NULL));
   // while(1)
    {
        sleep(rand()%2);
        //拿筷子
        take_chopsticks(i);
        for(int i = 0 ; i < N ;i++)
        {
            if(state[i] == 1 && state[(i+1)%N] == 1) ;
            else
            {
                printf("philosopher %d is thinking!!!\n",i);
            }
        }
        printf("philosopher %d is eating!!!\n",i);
        //放筷子
        put_chopsticks(i);
        sleep(rand()%2);
    }
}

int main()
{
    pthread_t thr[5];
    pthread_mutex_init(&mutex,NULL);
    for(int i = 0; i < N ; i++)
    {
        pthread_create(&thr[i],NULL,philosopher,(void*)i);
    }

    for(int i = 0 ; i < N ; i++)
    {
        pthread_join(thr[i],NULL);
    }
    //销毁互斥锁
    pthread_mutex_destroy(&mutex);

    return 0;
}

