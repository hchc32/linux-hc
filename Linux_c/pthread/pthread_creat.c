#include <stdio.h>
#include<unistd.h>
#include<pthread.h>

void* thread_creat(void *arg)
{
    printf("I am a thread ! pid = %d\n",getpid());
    return NULL;
}
int main()
{
    pthread_t tid;
    pthread_create(&tid,NULL,thread_creat,NULL);
    printf("I am a main thread , pid = %d , tid = %lu\n",getpid(),pthread_self());
    sleep(1);
    return 0;
}

