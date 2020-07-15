#include <stdio.h>
#include<stdlib.h>

typedef  struct list
{
    int n;
    struct list *next;
}Node,*LinkList;
LinkList Creat(Node *head);
void  print(Node *head);
int main()
{
    Node *head;
    head=Creat(head);
    print(head);
    printf("\n");
    head=endhead(head);
    print(head);
    return 0;
}

LinkList Creat(Node *head)
{
    Node *p1,*p2;
    head=p1=(LinkList)malloc(sizeof(Node));
    p2=(LinkList)malloc(sizeof(Node));
    scanf("%d",&p2->n);
    while(p2->n!=-1)
    {
        p1->next=p2;
        p1=p2;
        p2=(LinkList)malloc(sizeof(Node));
        scanf("%d",&p2->n);
    }
    free(p2);
    p1->next=NULL;
    return head;
}
LinkList  endhead(Node *head)
{
    Node *tail,*p1,*p2;
    tail=p1=p2=head->next;
    p2=p2->next->next;
    p1=p1->next;
    while(p2!=NULL)
    {
        p1->next=head->next;
        head=p1;
        p1=p2;
        p2=p2->next;
    }
    return head;
}

void  print(Node *head)
{
    head=head->next;
    while(head!=NULL)
   {
        printf("--%d",head->n);
        head=head->next;
    }
}
