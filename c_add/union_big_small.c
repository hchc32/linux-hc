#include <stdio.h>

union test
{
    int b;
    char ch;
}s;

int main()
{
    s.b=1;
    //000000000001    0x 00 01   
    //小端 0x01    
    //大端 0x00
    if(s.ch == 1)
        printf("小端\n");
    else
        printf("大端\n");           
    return 0;
}

