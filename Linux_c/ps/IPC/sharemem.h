#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<errno.h>

#define SHM_SIZE 1024

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

//创建信号量函数
int createsem(const char* pathname,int proj_id,int members,int init_val)
{
    //member信号种类数
    key_t msgkey;
    //index = 特定的标识符
    int index,sid;
    union semun semopts;
    //获取键值
    if((msgkey = ftok(pathname,proj_id)) == -1)
    {
        perror("ftok error!\n");
        return -1;
    }
    //获取信号量标识符
    if((sid = semget(msgkey,members,IPC_CREAT|0666)) == -1)
    {
        perror("semget error!\n");
        return -1;
    }
    //初始化操作
    semopts.val = init_val;
    for(index = 0; index < members ;index++)
    {
        //SETVAL = 设定ｉｎｄｅｘ指定信号的值
        semctl(sid,index,SETVAL,semopts);
    }
    //返回一个信号集标识符
    return (sid);
}

//打开信号量函数
int opensem(const char*pathname,int proj_id)
{
    key_t msgkey;
    int sid;
    if((msgkey = ftok(pathname,proj_id)) == -1)
    {
        perror("ftok error!");
        return -1;
    }
    //第二个参数为0,表示打开一个已经存在的信号量
    if((sid = semget(msgkey,0,IPC_CREAT|0666)) == -1)
    {
        perror("semget error!");
        return -1;
    }
    return (sid);
}

//P操作函数
int sem_p(int semid,int index)
{
    struct sembuf buf = {0,-1,IPC_CREAT};
    if(index < 0)
    {
        perror("index of array cannot equals a minus value!");
        return -1;
    }
    buf.sem_num = index;
    if(semop(semid,&buf,1) == -1)
    {
        perror("a wrong operation to semaphore occurred!");
        return -1;
    }
    return 0;
}
//V操作函数
int sem_v(int semid,int index)
{
    struct sembuf buf = {0,1,IPC_CREAT};
    if(index < 0)
    {
        perror("index of array cannot equals a minus value!");
        return -1;
    }
    buf.sem_num = index;
    if(semop(semid,&buf,1) == -1)
    {
        perror("a wrong operation to semaphore occurred!");
        return -1;
    }
    return 0;
}

//删除信号集函数
int sem_delete(int semid)
{
    return (semctl(semid,0,IPC_RMID));
}

//等待信号为1
int wait_sem(int semid,int index)
{
    while(semctl(semid,index,GETVAL,0) == 0)
    {
        sleep(1);
    }
    return 1;
}

//创建共享内存函数
int createshm(char *pathname,int proj_id,size_t size)
{
    key_t shmkey;
    int sid;

    //获取键值
    if((shmkey = ftok(pathname,proj_id)) == -1)
    {
        perror("ftok error!\n");
        return -1;
    }

    if((sid = shmget(shmkey , size ,IPC_CREAT | 0666)) == -1)
    {
        perror("shmget call failed.\n");
        return -1;
    }
    return (sid);
}
