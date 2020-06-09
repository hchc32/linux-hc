#include <stdio.h>
#include<stdlib.h>

typedef char QueueType;

//定义结点
typedef struct node
{
    QueueType data;
    struct node* next;
}node;

typedef struct queue_link
{
    node* head;
    node* end;
}que;

//初始化队列
que* InitQueue()
{
    que* q = (que*)malloc(sizeof(que));
    q->head = q->end = NULL;
    return q;
}

//判断队列是否为空
int EmptyQueue(que *q)
{
    return q->end == NULL;
}

//入队
void Push(que *q,QueueType data)
{
    node * temp = (node*)malloc(sizeof(node));
    if(temp == NULL)
    {
        perror("malloc error!");
        return ;
    }
    temp->data = data;
    if(q->end == NULL)
    {
        temp->next = q->end;
        q->head = temp;
        q->end = temp;
    }
    else
    {
       //尾插法
       q->end->next = temp;
       //队尾指向改变
       q->end = temp;
    }
}

//出队
void Pop(que* q)
{
    node *temp = q->head;
    if(temp == NULL)
    {
        return ;
    }
    //队列只有一个元素的时候
    else if(q->end == q->head)
    {
        q->head = NULL;
        q->end = NULL;
        free(temp);
    }
    else
    {
        q->head = q->head->next;
        free(temp);
    }
}

//打印队列
void Display(que* q)
{
    node *temp = q->head;
    if(temp == NULL)
    {
        return ;
    }
    while(temp != NULL)
    {
        printf("%c ",temp->data);
        temp = temp ->next ;
    }
    printf("\n");
}

int main()
{
    int n = 5; 
    que *q = InitQueue();
    printf("开始入队列:\n");
    while(n--)
    {
        printf("%c入队，队列为: ",'A'+n);
        Push(q,'A'+n);
        Display(q);
    }
    printf("开始出队列:\n");
    int j = 5;
    while(j--)
    {
        printf("元素出队,队列为:\n");
        Pop(q);
        Display(q);
    }
    printf("\n");
    return 0;
}
