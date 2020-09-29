#ifndef STACK_H_
#define STACK_H_

typedef struct fangxiang
{
    int x;
    int y;
    int fx;
}Pos;

typedef Pos DataType;


//栈的数据结点
typedef struct StackNode
{
    DataType data;
    struct StackNode* next;
}StackNode,*LinkStackptr;

//栈的元素
typedef struct LinkStack
{
    LinkStackptr  top;
    int size_stack;
}LinkStack;

//初始化一个空栈
LinkStack* Init_Stack();

//判断栈是否是空栈
int Is_Empty(LinkStack* head);

//清空栈
void Clear_Stack(LinkStack* head);

//若栈存在,销毁栈
LinkStack* Destroy_Stack(LinkStack *head);

//若栈不为空,获取栈顶的元素
void Get_Top(LinkStack *head , DataType* top_data);

//若栈存在，将元素入栈
int  Push(LinkStack *head , DataType push_data);

//若栈存在，将元素出栈
int Pop(LinkStack *head , DataType *pop_data);
#endif
