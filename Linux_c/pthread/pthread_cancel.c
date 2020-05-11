#include <stdio.h>
#include<pthread.h>
#include<unistd.h>

void *thr(void *arg)
{
    while(1)
    {
        int a;
        a=a+1;
        //pthread_testcancel();//强行设置取消点
        //里面不写代码,cancel杀死不了,因为没有取消点
    //    printf("I am thread,very happy! tid = %lu\n",pthread_self());
     //   sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thr,NULL);
    sleep(5);
    pthread_cancel(tid);
    void *ret;
    pthread_join(tid,&ret);
    printf("thread exit with %d\n",(int)ret);
    return 0;
}

