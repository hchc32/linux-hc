#include <stdio.h>
#define SIZE 1000
char * mystrncpy(char *a, char *b, int n); 

int main(void)
{
	char a[SIZE];
	char b[SIZE];
	int len;
	while(fgets(a, SIZE, stdin) && a[0] != '\0')
	{
		scanf("%d", &len);
		getchar();
        mystrncpy(a,b,len);
		puts(b);
	}
	
	return 0;	
}
char *mystrncpy(char *a, char *b, int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        *b = *a;
        a++;
        b++;   
    }
    if(n <= 0)
    {
        *b = '\0';
    }
    return b;
}
