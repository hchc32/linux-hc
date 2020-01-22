# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# define LEN 10
int main(void)
{
	FILE *fp ;
	char words[LEN] ;
	long num;
	if((fp=fopen("words","wb"))==NULL)
	{
		fprintf(stderr,"fpfile open error!");
		exit(EXIT_FAILURE);
	}
    num=0;
	while (fscanf(fp,"%ld %s",&num,words)>0);
	while ((fscanf(stdin,"%40s",words))==1 && (words[0]!='#'))
		fprintf(fp,"%ld %s",++num,words); 
	puts("File contents");
	rewind(fp);
	while (fscanf(fp,"%ld %s",&num,words)==2)
		fprintf(stdout,"%ld %s",num,words);
	fclose(fp);
    return 0 ;
}
