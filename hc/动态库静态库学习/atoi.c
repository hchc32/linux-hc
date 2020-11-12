#include <stdio.h>
#include"atoi.h"

int atoi(const char* str)
{
    int ret = 0;
    if(str != NULL)
    {
        sscanf(str,"%d",&ret);
    }
    return ret;
}

