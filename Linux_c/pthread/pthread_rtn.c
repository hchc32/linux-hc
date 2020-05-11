#include <stdio.h>
#include<unistd.h>
#include<pthread.h>

void *thr(void *arg)
{
    printf("I am thread , tid = %lu\n",pthread_self());
    sleep(5);
    printf("I am thread , tid = %lu\n",pthread_self());
    pthread_exit((void *)101);
    return (void *)100;

}

int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thr,NULL);
    void *ret;
    //线程回收
    pthread_join(tid,&ret);
    printf("ret exit with %d\n",ret);

    pthread_exit(NULL);

    return 0;
}

