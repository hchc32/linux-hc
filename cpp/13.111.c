# include <stdio.h>
# include <stdlib.h>
int main(void)
{
    int ch ;
    FILE * fp ;
    char st[100];
    unsigned long count = 0 ;
    printf("Please enter filename:");
    scanf("%s",st);
    if ((fp = fopen(st , "r")) == NULL)
    {
        printf("Can't open %s\n" , st);
    	exit(EXIT_FAILURE);
    }
    while ((ch=getc(fp))!=EOF)
    {
    	putc(ch , stdout);
    	count++ ;
    }
    fclose(fp);
    printf("File %s has %lu characters\n" ,st,count);
    return 0 ;
    } 
