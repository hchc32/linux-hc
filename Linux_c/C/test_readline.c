#include <stdlib.h>
#include<stdio.h>
#include <readline/readline.h>

int main(void)
{
    while (1)
    {
        char * str = readline("Myshell $ ");
        free(str);
    }
}
