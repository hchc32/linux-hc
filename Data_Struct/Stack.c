#include <stdio.h>
#include<stdlib.h>
//栈的数据结点
typedef struct StackNode
{
    int data;
    struct StackNode* next;
}StackNode,*LinkStackptr;

//栈的元素
typedef struct LinkStack
{
    LinkStackptr  top;
    int size_stack;
}LinkStack;

//初始化一个空栈
LinkStack* Init_Stack()
{
    LinkStack* head =(LinkStack* )malloc(sizeof(LinkStack));
    head->size_stack = 0;
    head->top = NULL;
    return head;
}

//判断栈是否是空栈
int Is_Empty(LinkStack* head)
{
    if(head == NULL)
    {
        printf("栈不存在!\n");
        return 1;
    }
    else
    {
        if(head->size_stack == 0)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
}


//清空栈
void Clear_Stack(LinkStack* head)
{
    if( Is_Empty(head) )
    {
        return;
    }
    LinkStackptr temp;
    while( head->size_stack-- )
    {
        temp = head->top->next;
        free(head->top);
        head->top = temp;
    }
}

//若栈存在,销毁栈
void Destroy_Stack(LinkStack *head)
{
    if(head == NULL)
    {
        return;
    }
    Clear_Stack(head);
    free(head);
    head = NULL;
}

//若栈不为空,获取栈顶的元素
void Get_Top(LinkStack *head , int* top_data)
{
    if( Is_Empty(head) )
    {
        printf("栈为空!\n");
        return;
    }
    else
    {
        *top_data = head->top->data;
        return;
    }
}

//若栈存在，将元素入栈
int  Push(LinkStack *head , int push_data)
{
    if(head == NULL)
    {
        printf("栈不存在\n");
        return 0;
    }
    LinkStackptr push_temp = (LinkStackptr)malloc(sizeof(StackNode));
    push_temp->data = push_data;
    push_temp->next = head->top;
    head->top = push_temp;
    head->size_stack ++;
    return 1;
}

//若栈存在，将元素出栈
int Pop(LinkStack *head , int *pop_data)
{
    LinkStackptr pop_temp; 
    if(head == NULL)
    {
        printf("栈不存在\n");
        return 0;
    }
    *pop_data = head->top->data;
    pop_temp = head->top;
    head->top = head->top->next;
    free(pop_temp);
    head->size_stack --;
    return 1;
}

