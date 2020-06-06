#include <stdio.h>
#include<sys/types.h>
#include<sys/sem.h>

#include<linux/sem.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<unistd.h>

#define MAX_RESOURCE 5
union semun1 {
    int              val;  
    struct semid_ds *buf;  
    unsigned short  *array;
    struct seminfo  *__buf;
                           
};

int main()
{
    key_t key;
    int semid;
    struct sembuf sbuf = {0,-1,IPC_NOWAIT};
    union semun1 semopts;
    if((key = ftok(".",'s')) == -1)
    {
        perror("ftok error!\n");
        exit(0);
    }
    if((semid = semget(key,1,IPC_CREAT|0666) == -1))
    {
       perror("semget error!");
       exit(0);
    }

    semopts.val = MAX_RESOURCE;
    if((semctl(semid,0,SETVAL,semopts)) == -1)
    {
        perror("semctl error!");
        exit(0);
    }

    while(1)
    {
        if(semop(semid,&sbuf,1) == -1)
        {
            perror("semop error!");
            exit(0);
        }
        sleep(3);
    }
    
    return 0;
}

