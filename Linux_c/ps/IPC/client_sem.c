#include <stdio.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<linux/sem.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<unistd.h>

#define MAX_RESOURCE 5

int main()
{
    key_t key;
    int semid,semval;
    union semun semopts;
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
    while(1)
    {
        if((semval = semctl(semid,0,GETVAL,0)) == -1)
        {
            perror("semop error!");
            exit(0);
        }
        if(semval > 0)
        {
            printf("Still %d resources can be used\n",semval);
        }
        else
        {
            printf("No more resources can be used\n");
            break;
        }
        sleep(3);
    }
    
    return 0;
}

