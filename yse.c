#include <stdio.h>
#include<stdlib.h>
struct list
{
    int n;
    struct list *next;
};
struct list* creat();
void joseph(struct list *p,int N,int m);

int main()
{   
    struct list *head;
    int m,N;
    head=creat();
    printf("从第几个人开始报数:");
    scanf("%d",&N);
    printf("请输入要出列的数字:");
    scanf("%d",&m);
    joseph(head,N,m);
    return 0;
}

struct list * creat()
{
	struct list *p1,*p2,*head;
	head=p1=(struct list*)malloc(sizeof(struct list));
	p2=(struct list*)malloc(sizeof(struct list));
	printf("请输入你要输入的数:(以0结尾)");
    scanf("%d",&head->n);
    if(head->n==0)
        return NULL;
	scanf("%d",&p2->n);
	while(p2->n!=0)
	{
		p1->next=p2;
		p1=p2;
		p2=(struct list*)malloc(sizeof(struct list));
		scanf("%d",&p2->n);
	}
	free(p2);
	p1->next=head;
	return head;
}
void joseph(struct list *p,int N,int m)
{
    struct list *p1=p;
    struct list *p2=p;
    while(p1->n!=N)
    {
        p2=p1;
        p1=p1->next;
    }
    int flag=m;
    while(p1->next!=p1)
    {
        while(m>=2)
        {
            p2=p1;
            p1=p1->next;
            --m;
        }
        p2->next=p1->next;
        printf("出列的人为%d",p1->n);
        free(p1);
        p1=p2->next;
        m=flag;
    }
    printf("获胜的人为%d",p1->n);
}
