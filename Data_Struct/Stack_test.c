#include <stdio.h>
#include"Stack.c"

int main()
{
    LinkStack* head = Init_Stack();
    for(int i = 0; i < 5; i++)
    {
        int push_data;
        scanf("%d",&push_data);
        Push(head,push_data);
    }
    printf("栈的大小为%d\n",head->size_stack);
    printf("将栈的元素全部弹出为:");
    int n = head->size_stack;
    for(int i = 0; i < n; i++)
    {
        int pop_data;
        Pop(head,&pop_data);
        printf("%d ",pop_data);
    }

    Destroy_Stack(head);
    if(head == NULL)
    {
        printf("栈不存在了\n");
    }
    else
    {
        printf("栈存在\n");
    }
    return 0;
}
