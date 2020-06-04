#include<stdio.h>
#include<sys/types.h>
#include<sys/ipc.h>


int main()
{
    int i;
    for(i = 0 ; i <= 5; i++)
    {
        printf("key[%d] = %ul \n",i,ftok(".",i));
    }
    return 0;
}

