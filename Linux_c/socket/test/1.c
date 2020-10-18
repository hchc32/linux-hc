#include<stdio.h>
#include<stdlib.h>
typedef  struct
{
 char * ch;
 int len;
}HString;

void HStrInit(HString *s)
{

  s=(HString *)malloc(sizeof(HString));
 s->ch=NULL;
 s->len=0;
}

int HStrAssign(HString **s, char* a)
{
 int i=0;
 while(a[i]!='\0')
  i++;
 (*s)->len=i;
 if((*s)->len!=0)
 {
  (*s)->ch=(char *)malloc(((*s)->len+1)*sizeof(char));
  if((*s)->ch==NULL) return 0;
  for(i=1;i<=((*s)->len);i++)
  (*s)->ch[i]=a[i-1];
 
  }
  else (*s)->ch=NULL;
  return 1;
}
void PrintHString(HString *s)
{
 if(s->ch==NULL)
 printf("赶紧去输一个吧");
 else
  for(int i=0;i<=s->len;i++)
  printf("%c",s->ch[i]);
}

int main()
{
 HString *s;
 HStrInit(s);
 HStrAssign(&s,"helloworld");
 PrintHString(s);
 return 0;
}
