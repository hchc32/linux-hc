#include <stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>

void* thread_creat(void *arg)
{
    //打印子线程的ID
    printf("I am a thread ! tid = %lu\n",pthread_self());
    return NULL;
}
int main()
{
    pthread_t tid;
    //初始化线程的属性
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    //设置分离
    pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    //创建线程的时候设置线程分离int ret = pthread_create(&tid,NULL,thread_creat,NULL);
    int ret = pthread_create(&tid,&attr,thread_creat,NULL);
    {
        printf("error number: %d\n",ret);
        printf("%s\n",strerror(ret));
    }
    printf("I am a main thread , pid = %d , tid = %lu\n",getpid(),pthread_self());
    for(int i = 0 ; i < 5; i++)
    {
        printf("i = %d\n", i);
    }
    sleep(2);
    //释放资源
    pthread_attr_destroy(&attr);
    return 0;
}

