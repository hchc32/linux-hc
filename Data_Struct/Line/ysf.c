#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 100

typedef struct NodeType
{
    //人员编号
    int id;
    //所持密码
    int password;
    struct NodeType* next;
}NodeType;


void CreatCList(NodeType **head , int n)
{
    int password = 0;
    NodeType* pnew = NULL;
    NodeType* pcur = NULL;
    (*head) = (NodeType*)malloc(sizeof(NodeType));
    //printf("请输入第0个人的密码: ");
    (*head)->id = 0;
    scanf("%d",&(*head)->password);
    (*head)->next = *head;
    if(n == 1)
    {
        return;
    }
    pcur = (*head); 
    for(int i = 1 ; i < n ; i++ )
    {
        //printf("请输入第%d个人的密码: ",i);
        scanf("%d",&password);
        pnew = (NodeType*)malloc(sizeof(NodeType));
        pnew->id = i;
        pnew->password = password;
        pcur->next = pnew;
        pnew->next = NULL;
        pcur = pnew;
    }
    pnew->next = (*head);
}

void PrintList(NodeType* head)
{
    NodeType* temp = head;
    printf("id       password\n");
    printf("%d        %d\n",temp->id,temp->password);
    temp = temp->next;
    while(temp != head)
    {
        printf("%d        %d\n",temp->id,temp->password);
        temp = temp->next;
    }
}


/*
int IsEmptyy(NodeType* head)
{
    if(head->next == head)
    {
        printf("The List is empty!");
        return 1;
    }
    return 0;
}
*/

void JposephusOperate(NodeType* head , int ipassword)
{
    int password = ipassword;
    NodeType* temp = NULL;
    //NodeType* pcur = head;
    int flag = 0;
    while(head->next != NULL)
    {
        if(head->next == head)
        {
            flag = 1;
        }
        //循环找到要删除的结点　
        for(int i = 0; i < password-1; i++)
        {
            temp = head;
            head = head->next;
        }
        //printf("第%d个人出列<密码:%d>\n",head->id,head->password);
        printf("%d ",head->id+1);
        if(head->next == head)
        {
            head->next = NULL;
        }
        password = head->password;
        NodeType* pdel = head;
        temp->next = head->next;
        head = temp->next;
        free(pdel);
        if(flag == 1)
        {
            break;;
        }
    }
    //然后删除结点，并保留这个结点的ｐａｓｓｗｏｒｄ，直到链表为空
}
int main()
{
    int n;
    int password;
    do
    {
        //printf("请输入人数:(不超过100)");
        scanf("%d",&n);
    }while(n>100);
    //printf("请输入初始密码:");
    scanf("%d",&password);

    NodeType *head;
    //创建n个结点的单向循环链表
    CreatCList(&head,n);
    //打印循环链表
    //PrintList(head);
    //进行约瑟夫主体逻辑
    JposephusOperate(head,password);
    return 0;
}
