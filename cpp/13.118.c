#include <stdio.h>
#include<stdlib.h>
int main(int agrc,char *argv[])
{   
    FILE *fp;
    if(argc==2)
    {
        printf("%c出现的次数为%d",argv[1],numfile(*argv[1],stdin));
    }


    return 0;
}

