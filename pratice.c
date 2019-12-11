#include<stdio.h>
#include<stdlib.h>

struct stu* creat(void);
void print(struct stu* p);
struct stu* find(struct stu* p);
int delete(struct stu *p0);
int insert(struct stu *p0);

struct stu
{
    long num;
    char name[20];
    float score;
    struct stu *next;
};

int main()
{
    struct stu* head,*p;
    head=creat();
    print(head);
    p=find(head);
    if(p)
        printf("找到学号:%ld 姓名:%-10s 成绩:%6.1f\n",p->num,p->name,p->score);
    else
        printf("没找到!");
    if(insert(head))
        printf("已成功插入\n");
    else
        printf("有重号，插入失败!");
    print(head);
    if(delete(head))
        printf("已经正确删除");
    else
        printf("要删除的结点不存在!\n");
    print(head);
    return 0;
}

struct stu* creat(void)
{
        //定义一个头指针
    struct stu* head,*p1,*p2;
    //head p2都指向头节点
    head=p2=(struct stu*)malloc(sizeof(struct stu));
    printf("请输入学号      姓名        成绩(学号输入０结束)\n");
    //p1指向头节点
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

int delete(struct stu *p0)      //要有两个结点，一个为找删除的，一个为重新链接的
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
    }
    return 0;
}

int insert (struct stu *p0)
{
    struct stu *p;
    p=(struct stu*)malloc(sizeof(struct stu));
    printf("请输入要插入的学号　　　姓名　　　成绩\n");
    scanf("%ld      %s      %f",&p->num,p->name,&p->score);
   // getchar();
    while(p0->next!=NULL  &&  p0->next->num < p->num )   //寻找要插入的位置
    {
       // printf("请输入要插入的学号　　　姓名　　　成绩\n");
       // getchar();
        p0=p0->next;
    }
    if(p0->next!=NULL  &&  p0->next->num == p->num)    //寻找是否有重号，有重号则不插入
    {
        free(p);                                        //未插入时要释放掉申请的内存空间
        return 0;
    }
    p->next=p0->next;
    p0->next=p;                    //已经插入，不能free掉p
    return 1;
}
