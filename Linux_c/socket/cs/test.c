#include <stdio.h>

int main()
{
    char* pread;
    char buf[10] = "mac";
    pread = buf;
    printf("%c\n",*pread++);
    int i;
    char data_buf[10];
    int len =100;
    int len_remain = 3;
    for( i = 0; *pread!='\0' ; i++)
    {
        if(i > len)
        {
            return -1;
        }
        data_buf[i] = *pread++;
        printf("%c\n",data_buf[i]);
        len_remain--;
    }

    return 0;
}

