#include <stdio.h>
#include<stdlib.h>

#define N 999999
typedef struct node
{
    int id;
    int password;
    struct node* next;
}Node;

int book[N];
int a[N];

//创建单向循环链表
void CreatCList(Node** head,int n)
{
    int count = 1;
    (*head) = (Node *)malloc(sizeof(Node));
    (*head)->password = count++;
    (*head)->id = 0;
    (*head)->next = (*head);
    Node* cur = (*head),* p = (*head);
    for(int i = 0 ; i < n-1 ; i++)
    {
        cur = (Node*)malloc(sizeof(Node));
        cur->password = count++;
        cur->next = p->next;
        p->next = cur;
        p = p->next;
    }
}
void PrintList(Node* head)
{
    Node* temp = head;
    printf("id       password\n");
    printf("%d        %d\n",temp->id,temp->password);
    temp = temp->next;
    while(temp != head)
    {
        printf("%d        %d\n",temp->id,temp->password);
        temp = temp->next;
    }
}


void JonesCir(Node* head,int m)
{
    Node* cur = head,* pdel = NULL;
    Node* phead;
    int flag = 0;
    int j = 0;
    while(cur->next != NULL)
    {
        if(cur->next == cur)
        {
            flag ++;
        }
        for(int i = 0 ; i < m-1 ; i++)
        {
            phead = cur;
            cur = cur->next;
        }
        pdel = cur;
        book[j++] = pdel->password; 
        phead->next = cur->next;
        cur = phead->next;
        free(pdel);
        if(flag > 0)
        {
            break;
        }
    }
}


int main()
{
    Node* head;
    int n,m,k;
    //n人数
    scanf("%d %d",&n,&k);
    for(int i = 0; i < k ; i++)
    {
        scanf("%d",&a[i]);
    }
    //创建单向循环链表　
    //PrintList(head);
    int count = 0;
    for(m = 0; m <= n; m++)
    {
        CreatCList(&head,n);
        JonesCir(head,m);
        int flag = 0;
        for(int j = n-k ; j <= n-1 ; j++)
        {
            printf("%d %d\n",a[count],book[j]);
            if(a[count++] == book[j])
            {
                flag++;
            }
            else
            {
                printf("0\n");
                return 0;
            }
        }
        printf("%d\n",m);
        return 0;
    }
}

