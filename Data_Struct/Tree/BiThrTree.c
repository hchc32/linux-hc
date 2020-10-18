#include <stdio.h>
#include<stdlib.h>
//线索二叉树的存储结构定义
typedef char TElemType;
typedef enum {Link,Thread} PointerTag; //Link 0 孩子 Thread 1 线索
typedef struct BiThrNode
{
    TElemType data;                     //结点数据
    struct BiThrNode *lchild, *rchild;  //左右孩子指针
    PointerTag LTag;                    //左右标志，是线索还是孩子
    PointerTag RTag;
}BiThrNode,*BiThrTree;
BiThrTree pre;                         //全局变量，始终指向刚刚访问过的结点

void CreatBiThrTree(BiThrTree * T);
void InThreading(BiThrTree T);
void InOrderThreading(BiThrTree *p,BiThrTree T);   //使得pre有初始值
int main()
{
    BiThrTree T = NULL,p;
    CreatBiThrTree( &T );
    InOrderThreading( &p,T );
    return 0;
}
//创建二叉树(前序遍历输入数据)
void CreatBiThrTree(BiThrTree * T)
{
    TElemType ch;
    scanf("%c",&ch);
    if( ch == ' ' )
        *T = NULL;
    else
    {
        *T = (BiThrNode *)malloc(sizeof(BiThrNode));
        (*T)->data = ch;
        (*T)->LTag = Link;
        (*T)->RTag = Link;
    CreatBiThrTree (&((*T)->lchild));
    CreatBiThrTree (&((*T)->rchild));
    }
}
//中序遍历线索化
void InThreading(BiThrTree T)
{
    if( T )
    {
        InThreading(T->lchild);     // 递归左孩子线索化
        // 结点处理
        if( !T->lchild )
        {
            T->LTag = Thread;
            T->lchild = pre;
        }
        if( !pre->rchild )          //gdb调试的时候会出错,pre刚开始赋值在第58行
        {
             pre->RTag = Thread;
             pre->rchild = T;
        }
        pre = T;
        InThreading(T->rchild); 
    }
}

void InOrderThreading(BiThrTree *p,BiThrTree T)
{
    *p = (BiThrTree)malloc(sizeof(BiThrNode));
    (*p)->LTag = Link;
    (*p)->RTag = Thread;
    (*p)->rchild = *p;
    if( !T )
    {
        (*p)->lchild = *p;
    }
    else
    {
        (*p)->lchild = T;
        pre = *p;
        InThreading(T);
        pre->rchild = *p;
        pre->RTag = Thread;
        (*p)->rchild = pre;
    }
}
