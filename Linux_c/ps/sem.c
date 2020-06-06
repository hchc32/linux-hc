#include <stdio.h>
#include<sys/sem.h>
#include<unistd.h>

int sem_p(int semid , int index)
{
    struct sembuf buf = {0,-1,IPC_NOWAIT};
    if(index < 0)
    {
        //数组的索引不能是负值
        perror("index of array cannot equals a minus value");
        return -1;
    }
    buf.sem_num = index;
    if(semop(semid , &buf ,1) == -1)
    {
        perror("a wrong operation to semaphore occurred! ");
        return -1;
    }
    return 0;
}

int sem_v(int semid,int index)
{
    struct sembuf buf = {0,1,IPC_NOWAIT};
    if(index < 0)
    {
        perror("index of array cannot equals a minus value");
        return -1;
    }

    buf.sem_num = index;
    if(semop(semid,&buf,1) == -1)
    {
        perror("a wrong operation to semaphore occurred! ");
        return -1;
    }
    return 0;
}

int main()
{

    return 0;
}

