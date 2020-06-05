#include <stdio.h>
#include<sys/sem.h>
#include<unistd.h>

int sem_p(int semid , int index)
{
    struct sembuf buf = {0,-1,IPC_NOWAIT};
    if(index < 0)
    {
        perror("index ");
        return -1;
    }
    buf.sem_num = index;
    if(semop(semid , &buf ,1) == -1)
    {
        perror("semop");
        return -1;
    }
    return 0;
}

int main()
{

    return 0;
}

