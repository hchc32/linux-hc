# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# define LEN 40
int main(void)
{
	FILE * in , * out ;
	int ch ;
	char name[LEN] ;
	int count = 0 ;
	char file[LEN] ;
	char * pch ;
	puts("Please enter file name :");
	scanf("%s" , file);
	if ((in = fopen(file , "r")) == NULL)
	{
		fprintf(stderr , "I couldn't open the file \"%s\"\n" ,file);
		exit(EXIT_FAILURE);
	}
	strncpy(name,file,LEN-5);
	name[LEN-5]='\0';
	if ((out = fopen(name,"w")) == NULL)
	{
		fprintf(stderr,"Can't create output file.\n");
		exit(3);
	}
	while ((ch=getc(in)) != EOF)
	{
		if (count++%3==0)
			putc(ch , out);
	}
	if (fclose(in)!=0 || fclose(out)!=0)
		fprintf(stderr,"Error in closing files\n");
	return 0 ;
}
