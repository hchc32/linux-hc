#include <iostream>
#include<stack>
#include<cstdlib>
#include<queue>

using namespace std;
typedef char TElemType;

typedef struct BiTNode
{
    TElemType data;
    struct BiTNode *lchild,*rchild;
}BiNode,*BiTree;


void CreatBiTree(BiTree *root)
{
    TElemType ch;
    scanf("%c",&ch);
    if(ch == '#')
    {
        (*root) = NULL;
    }
    else
    {
        (*root) = (BiTree)malloc(sizeof(BiNode));
        if((*root) == NULL)
        {
            printf("malloc error !");
            exit(1);
        }
        //(*root)->lchild = NULL;
        //(*root)->rchild = NULL;
        (*root)->data = ch;
    }
    CreatBiTree(&(*root)->lchild);
    CreatBiTree(&(*root)->rchild);
}

void visit(BiTree Node)
{
    printf("%d ",Node->data);
}

void PreOrder(BiTree root)
{
    stack<BiNode> S;
    BiTree p = root;
    while(p!=NULL && ! S.empty())
    {
        while(p!=NULL)
        {
            visit(p);
            S.push((*p));
            p = p->lchild;
        }
        if( !S.empty() )
        {
            p = &S.top();
            p = p->rchild;
        }
    }
}

void InOrderI(BiTree root)
{
    BiTree p;
    stack<BiNode> S;
    while(p!=NULL && !S.empty())
    {
        while(p!=NULL)
        {
            S.push(*p);
            p = p->lchild;
        }
        if(!S.empty())
        {
            p=&S.top();
            visit(p);
            p = p->rchild;
        }
    }
}


//二叉树的层次遍历
void LevelOrder(BiTree root)
{
    BiTree p = root;
    queue<BiNode> Q;
    Q.push(*root);
    while(!Q.empty())
    {
        p = &Q.front();
        visit(p);
        if(!p->lchild)
            Q.push(*p->lchild);
        if(!p->rchild)
            Q.push(*p->rchild);
    }
}

//统计二叉树的结点数
int TreeCount(BiTree root)
{
    static int count = 0;
    if(root)
    {
        count++;
        TreeCount(root->lchild);
        TreeCount(root->rchild);
    }
    return count;
}

int main()
{
    BiTree root;
    CreatBiTree(&root);
    //PreOrder(root);
    return 0;
}
