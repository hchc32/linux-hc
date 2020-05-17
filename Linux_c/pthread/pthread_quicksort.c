#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>
#include<unistd.h>

#define MAXNUM 1000000
#define PTHNUM 2
#define ARRNUM (MAXNUM/PTHNUM)
int q[MAXNUM];
int arry[MAXNUM];
pthread_barrier_t barr;


void quick_sort(int l,int r)
{
    if(l >= r) return ;
    int x = q[(l+r)>>1],i = l-1,j = r+1;
    while( i < j )
    {
        do i++; while(q[i] < x);
        do j--; while(q[j] > x);
        if( i < j )
        {
            int temp = q[i];
            q[i] = q[j];
            q[j] = temp;
        }
    }
    quick_sort(l,j);
    quick_sort(j+1,r);
}


void* pthread_work(void* args)
{
    int index = (int)args;
    quick_sort(index,index+ARRNUM);
    pthread_barrier_wait(&barr);
    pthread_exit(NULL);
}

void merge()
{
    long index[PTHNUM];
    for(long i=0;i<PTHNUM;i++)
        index[i]=i*ARRNUM;

    for(int i = 0;i < MAXNUM ;i++)
    {
         int min_index;
         int min_num=9999999;
         for(int j=0;j<PTHNUM;j++)
         {
             if((index[j]<(j+1)*ARRNUM)&&(q[index[j]]<min_num))
             {
                 min_num=q[index[j]];
                 min_index=j;
             }
         }
         arry[i]=q[index[min_index]];
         index[min_index]++;
    }

}

int main()
{
    //给数组随机赋值
    for(int i = 0; i < MAXNUM ; i++)
    {
        q[i] = random();
    }
    pthread_barrier_init(&barr,NULL,PTHNUM);
    pthread_t pth[PTHNUM];
    //创建线程
    for(int i = 0; i < PTHNUM ; i++)
    {
        pthread_create(&pth[i],NULL,pthread_work,(void*)(i*ARRNUM));
    }
    //等待所有线程在统一起跑线上
    pthread_barrier_wait(&barr);
    merge();

    /*
    //回收线程
    for(int i = 0; i < PTHNUM ; i++)
    {
        pthread_join(pth[i],NULL);
    }
    */
    return 0;
}

