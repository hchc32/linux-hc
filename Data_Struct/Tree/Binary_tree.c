//参考大话数据结构
//创建二叉树 
#include <stdio.h>
#include<stdlib.h>
typedef char TElemType; 

typedef struct BiTNode
{
    TElemType data;                    // 结点数据
    struct BiTNode *lchild , *rchild;   // 左右孩子指针
}BiNode,*BiTree;

//创建二叉树
void CreatBiTree (BiTree *T);
//二叉树的前序遍历
void PreOrderTraveres (BiTree T);
//二叉树的中序遍历
void InOrderTraveres (BiTree T);
//二叉树的后序遍历
void PostOrderTraveres (BiTree T);


int main()
{
    BiTree T;
    CreatBiTree ( &T );    
    return 0;
}


// 建立二叉树
void CreatBiTree (BiTree *T)
{
    TElemType ch;
    scanf("%c",&ch);
    getchar();
    if(ch == '#')
        *T = NULL;
    else
    {
        *T = (BiTree)malloc(sizeof(BiNode));
        if( !T  )
            exit(-1);
        (*T)->data = ch; 
    }
    CreatBiTree (&((*T)->lchild));     // 构建左子树
    CreatBiTree (&(*T)->rchild);       // 构建右子树
}

// 前序遍历二叉树
void PreOrderTraveres (BiTree T)
{
    if(T == NULL)
        return ;
    printf("%c",T->data);
    PreOrderTraveres (T->lchild);
    PreOrderTraveres (T->rchild);
}
// 中序遍历二叉树
void InOrderTraveres (BiTree T)
{
    if(T == NULL)
        return ;
    PreOrderTraveres (T->lchild);
    printf("%c",T->data);
    PreOrderTraveres (T->rchild);
}
// 后序遍历二叉树
void PostOrderTraveres (BiTree T)
{
    if(T == NULL)
        return ;
    PreOrderTraveres (T->lchild);
    PreOrderTraveres (T->rchild);
    printf("%c",T->data);
}

