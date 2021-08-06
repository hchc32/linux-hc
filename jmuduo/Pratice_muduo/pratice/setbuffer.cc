#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_LINE 64*1024
 
char buffer[MAX_LINE];
 
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("%s file_name\n", argv[0]);
        return 1;
    }
    const char *file_name = argv[1];
    FILE *fp = fopen(file_name, "ae");
    setbuffer(fp, buffer, sizeof(buffer));
 
    char str[1024];
    memset(str, 'a', sizeof(str)-1);
    str[1024-1] = '\n';
    for (int i = 0; i < 100; i++)
    {
        printf("i = %d\n", i);
        fwrite(str, 1, sizeof(str), fp);
        sleep(1);
    }
 
    getchar();
}