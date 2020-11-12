#include <stdio.h>
#include<stdlib.h>

typedef int TypeName;

typedef struct node
{
    TypeName data;
    struct node *next;
}Node,*LinkList;

typedef struct list
{
    LinkList ListNode;
    int ListLen;
}List;


/*
//Init LinkList
List* InIt_LinkList()
{
    List *list = (List*)malloc(sizeof(List));
    list->ListLen = 0;
    list->ListNode = NULL;
    return list;
}
*/

LinkList GetListValue(LinkList head)
{
    int temp;
    int n;
    head = (LinkList)malloc(sizeof(Node));
    LinkList p = head;
    head->next = NULL;
    scanf("%d",&n);
    while(n--)
    {
        Node* new1 = (Node*)malloc(sizeof(Node)); 
        new1->next = NULL;
        scanf("%d",&temp);
        new1->data = temp;
        new1->next = p->next;
        p->next = new1;
        p = new1;
    }
    return head;
}

void PrintList(LinkList head)
{
    head = head->next;
    while(head!=NULL)
    {
        printf("%d ",head->data);
        head = head->next;
    }
}

int FindESData(LinkList head,int n)
{
    if( n <= 0 )
    {
        printf("n error!");
        exit(1);
    }
    LinkList slow , fast;
    fast = head;
    slow = head->next;
    while( n-- )
    {
        fast = fast->next;
        if(fast == NULL)
        {
            return -1;
        }
    }
    while(1)
    {
        if(fast->next == NULL)
        {
            return slow->data;
        }
        fast = fast->next;
        slow = slow->next;
    }
}

int main()
{
    LinkList head = NULL;
    head = GetListValue(head);
    PrintList(head);
    int n;
    while(1)
    {
        printf("请输入要查找倒数第一个数字:");
        scanf("%d",&n);
        printf("%d",FindESData(head,n));
    }
    return 0;
}

