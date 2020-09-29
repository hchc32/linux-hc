#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#define N 100000

typedef int ElemType;

typedef struct 
{
    ElemType elem[N];
    int lefttop;
    int righttop;
}DupsqStack;


int Init_DupsqStack(DupsqStack** s)
{
    if((*s = (DupsqStack*)malloc(sizeof(DupsqStack))) == NULL)
        return -1;
    (*s)->lefttop = -1;
    (*s)->righttop = N;
    return 0;
}

int Push_DupsqStack(DupsqStack* s,char status,ElemType x)
{
    //将数据元素ｘ压入左栈或右栈
    if(s->lefttop+1 == s->righttop )
    {
        return -1;
    }
    if(status == 'L')
    {
        s->elem[s->lefttop+1] = x;
        s->lefttop++;
    }
    else if(status == 'R')
    {
        s->elem[s->righttop-1] = x;
        s->righttop--;
    }
    else
    {
        return -1;
    }
    return 0;
}

ElemType Pop_DupsqStack(DupsqStack* s,char status)
{
    if(status == 'L')
    {
        if(s->lefttop < 0)
        {
            return NULL;
        }
        int temp =  s->elem[s->lefttop];
        s->lefttop--;
        return temp;
    }
    else if(status == 'R')
    {
        if(s->lefttop > N)
        {
            return NULL;
        }
        int temp = s->elem[s->righttop];
        s->righttop++;
        return temp;
    }
    else
    {
        return NULL;
    }
}


int main()
{
    DupsqStack *s;
    Init_DupsqStack(&s);    
    Push_DupsqStack(s,'L',14);
    Push_DupsqStack(s,'L',12);
    printf("%d",Pop_DupsqStack(s,'L'));
    printf("%d",Pop_DupsqStack(s,'L'));
    return 0;
}

