#include <stdio.h>
#include<stdlib.h>

struct stu* creat();
void print(struct stu *p);
struct stu *es(struct stu *head);

struct stu
{
    int n;
    struct stu*next;
};

int main()
{
    struct stu *head;
    head=creat();
    print(head);
    es(head);
    print(head);
    return 0;
}



struct stu* creat()
{
    struct stu *head;
    head=(struct stu*)malloc(sizeof(struct stu));
    head->next=NULL;
    struct stu *p2;
    p2=(struct stu*)malloc(sizeof(struct stu));
    int n;
    scanf("%d",&n);
    while(n!=-1)
    {
        p2->n=n;
        p2->next=head->next;
        head->next=p2;
        p2=(struct stu*)malloc(sizeof(struct stu));
        scanf("%d",&n);
    }
    free(p2);
    return head;
}

void print(struct stu *p)
{
    p=p->next;
    while(p!=NULL)
    {
        printf("%d ",p->n);
        p=p->next;
    }

}


struct stu *es(struct stu *head)
{
    struct stu *p1,*p2;
    p1=p2=head->next;
    head->next=NULL;
    while(p1!=NULL)
    {
        p1=p2->next;
        p2->next=head->next;
        head->next=p2;
        p2=p1;
    }
}

