#include <iostream>
#include<cstdlib>

typedef struct node
{
    int data;
    struct node* next;
}Node,*LinkList;

void CreatLinkList(LinkList* head)
{
    int NodeCount;
    (*head) = (LinkList)malloc(sizeof(Node));
    if( !(*head) )
    {
        printf("malloc error !");
        exit(1);
    }
    (*head)->next = NULL;
    LinkList p = (*head);
    LinkList q;
    printf("请输入链表结点的个数:");
    scanf("%d",&NodeCount);
    for(int i = 0; i < NodeCount ; i++)
    {
        q = (LinkList)malloc(sizeof(Node));
        if( !q )
        {
            printf("malloc error !");
            exit(1);
        }
        scanf("%d",&q->data);
        q->next = p->next;
        p->next = q;
        //
        p = p->next;
    }
}

void DisplayLinkList(LinkList head)
{
    if( !head || head->next == NULL)
    {
        printf("LinkList is NULL !");
        return;
    }
    head = head->next;
    while(head)
    {
        printf("%d ",head->data);
        head = head->next;
    }
}

Node * DataSort(Node *List)
{
    Node * p, * q, * tail;

    tail = NULL;
    while((List->next->next) != tail)
    {
        p = List;
        q = List->next;
        while(q->next != tail)
        {
            if((q->data) > (q->next->data))
            {
                p->next = q->next;
                q->next = q->next->next;
                p->next->next = q;
                q = p->next;
            }
            q = q->next;
            p = p->next;
        }
        tail = q;
    }
    return List;
}

void fun(LinkList head , LinkList Odd)
{
    LinkList Even = head;
    LinkList find = head->next;
    LinkList find_pre = head;
    LinkList Odd_temp = NULL;
    LinkList Even_temp = NULL;
    int flag_odd = 0;
    int flag_even = 0;
    while(1)
    {
        if( !find ) break;
        if(find->data % 2 == 0)
        {
            find_pre->next = find->next;
            find->next = NULL;
            if( !flag_odd )
            {
                Odd = find; 
                Odd_temp = Odd;
                flag_odd++;
            }
            else
            {
                Odd_temp->next = find;
                Odd_temp = Odd_temp->next;
            }
        }
        else
        {
            find_pre->next = find->next;
            find->next = NULL;
            if( !flag_even )
            {
                Even = find;
                Even_temp = Even;
                flag_even++;
            }
            else
            {
                Even_temp->next = find;
                Even_temp = Even_temp->next;
            }
        }
        find = find_pre->next;
    }
    printf("\n");
    printf("Odd:");
    if(Odd!=NULL)
    {
        printf(" %d ",Odd->data);
        DisplayLinkList(Odd);
    }
    printf("\n");
    printf("Even:");
    if(Even!=NULL)
    {
        printf(" %d ",Even->data);
        DisplayLinkList(Even);
    }
}
int main()
{
    LinkList head;
    LinkList Odd;
    CreatLinkList(&head);
    //DisplayLinkList(head);
    head = DataSort(head);
    DisplayLinkList(head);
    Odd = NULL; 
    fun(head,Odd);
    //DisplayLinkList(head);
}
