#include<stdio.h> 
#include<stdlib.h>
#define  LEN 50
struct stu* creat(void);
void print(struct stu* p);
struct stu* find(struct stu* p);
int      de(struct stu *p0);
int insert(struct stu *p0);
int writetofile(char *file,struct stu *head);
int readfromfile(char *file,struct stu *head);
int openfile();
struct stu
{
    long num;
    char name[20];
    float score; 
    struct stu *next;
};
char f[]="/home/hc/桌面/1.text";

int main()
{
    struct stu *head=NULL,*p=NULL;
    int select=1;
   // openfile();
 //   if(readfromfile(f,head));
   //     printf("读入数据成功");
    while(1)
    {
        printf("**************************\n");
        printf("*****学生信息管理系统*****\n");
        printf("*******1.录入信息*********\n");
        printf("**************************\n");
        printf("*******2.查询信息*********\n");
        printf("**************************\n");
        printf("*******3.删除信息*********\n");
        printf("**************************\n");
        printf("*******4.插入信息*********\n");
        printf("**************************\n");
        printf("*******5.显示信息*********\n");
        printf("**************************\n");
        printf("*******0.退出保存*********\n");
        printf("**************************\n");
        scanf("%d",&select);
        switch(select)
        {
            case 1://录入信息
                {
                    head = creat();
                    break;
                }
            case 2://查询信息
                {
                    p=find(head);
                    if(p)
                        printf("找到学号:%ld 姓名:%-10s 成绩:%6.1f\n",p->num,p->name,p->score);
                    else
                        printf("没找到!\n");
                    break;
                }
            case 3://删除信息
                {
                    if(de(head))                            
                        printf("已经正确删除\n");
                    else
                        printf("要删除的结点不存在!\n");
                    print(head);
                    break;
                }
            case 4://插入信息
                {
                    if(insert(head))                   
                        printf("已成功插入\n");
                  //  else
                      //  printf("有重号，插入失败!\n");
                    print(head);
                     break;
                }
            case 0:writetofile(f,head);return 0;
            case 5:
                   {
                       if(head==NULL)
                            printf("无信息，显示失败！");           
                       else
                       {
                            print(head);
                            break;
                       }
                   }
            default:
                   {
                       printf("请输入正确的选项:\n");
                       getchar();
                       continue;
                   }
        }
    }
    return 0;
}
struct stu* creat(void)
{
        //定义一个头指针
    struct stu* head,*p1,*p2;
    //head p2都指向头节点
    head=p2=(struct stu*)malloc(sizeof(struct stu));
    printf("请输入学号      姓名        成绩(学号输入０结束)\n");
    p1=(struct stu*)malloc(sizeof(struct stu));
    scanf("%ld %s %f",&p1->num,p1->name,&p1->score);
    while(p1->num!=0)
    {
        p2->next=p1;           //将新结点链接到表尾
        p2=p1;                 //p1指向表尾
        p1=(struct stu*)malloc(sizeof(struct stu));    //p1指向新申请的结点
        scanf("%ld %s %f",&p1->num,p1->name,&p1->score);
    }
    p2->next=NULL;
    free(p1);
    return(head);
}

void print(struct stu* p)
{
    p=p->next;
    while(p!=NULL)
    {
        printf("学号:%ld　姓名:%-10s 成绩:%6.1f\n",p->num,p->name,p->score);
        p=p->next;
        getchar();
    }
}

struct stu* find(struct stu* p)
{
    long num;
    printf("请输入要查找的学号:");
    scanf("%ld",&num);
    while(p->next!=NULL)
    {
        p=p->next;
        if(p->num==num)
        {
            return(p);
        }
    }
    return(NULL);
}

int de(struct stu *p0)      //要有两个结点，一个为找删除的，一个为重新链接的
{
    long num;
    struct stu* p;
    p=p0->next;
    if(p==NULL) return 0;     //头结点为空时，不能删除直接返回0
    printf("请输入要删除的学号:");
    scanf("%ld",&num);
    while(p!=NULL)
    {
        if(p->num == num)
        {
            p0->next=p->next;
            free(p);         //释放要删除的结点
            return 1;
        }
        p0=p;
        p=p->next;
    }
    return 0;
}

int insert (struct stu *p0)
{
    struct stu *p;
    int flag=0;
    p=(struct stu*)malloc(sizeof(struct stu));
    printf("请输入要插入的学号　　　姓名　　　成绩\n");
    scanf("%ld      %s      %f",&p->num,p->name,&p->score);
    p0=p0->next;
    while(p0->next!=NULL)
    {
        if(p->num<=p0->num)
        {
            p->next=p0->next;
            p0->next=p;
            flag=1;              //已经插入，不能free掉p
            return 1;
        }
    }
    if(flag==0)
    {   
        p0->next=p;
        p->next=NULL;
        return 1;
    }
}

int readfromfile(char *file,struct stu *p)
{
    int i;
    FILE *fp;
    fp=fopen(file,"r");
    if(fp==NULL)
    {
        printf("文件打开失败!");
        return 0;
    }
    while(!feof(fp))
    {
        fread(p,67,1,fp);
    }
    fclose(fp);
    printf("文件在%s处读取完毕",f);
    return 1;
}

/*int openfile()
{
   char filename[LEN]="/home/hc/桌面/1.text";
   FILE *fp;
   printf("请输入文件的路径:");
   scanf("%s",filename);
   fp=fopen(filename,"aw");
   if(fp == NULL)
   {
       printf("Open File Error!");
       return 0;
   }
   printf("%s文件打开成功!",filename);
   return 1;
}*/


int writetofile(char *file,struct stu *head)
{
    struct stu *p;
	FILE *fp;
	fp=fopen(file,"wr");
	if(fp==NULL)
	{
		printf("文件打开失败！");
		return 0;
	}
	p=head->next;
	while(p!=NULL)
	{
		fprintf(fp,"%ld %s %f\n",p->num,p->name,p->score);
        p=p->next;
	}
	fclose(fp);
	printf("\n学生信息保存完毕,保存在%s处!\n",f);
    return 1;
}
