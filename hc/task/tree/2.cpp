//判断一个树是不是另一个树的子数
#include <iostream>

using namespace std;
#define THREAD 1
#define LINK   0

typedef struct TNode
{
    char data;
    int Ltag;
    int Rtag;
    struct TNode *lchild;
    struct TNode *rchild;
}BiNode,*BiTree;

BiTree pre;

void CreatTree(BiTree *root)
{
    char temp_data;
    cin >> temp_data;
    if(temp_data != '#')
    {
        *root = (BiTree)malloc(sizeof(BiNode));
        (*root)->data = temp_data;
        (*root)->Ltag = LINK; 
        (*root)->Rtag = LINK; 
        CreatTree(&(*root)->lchild);
        CreatTree(&(*root)->rchild);
    }
    else
    {
        *root = NULL;
    }
}

void InThreading(BiTree ro)
{
    BiTree root  = ro;
    if( root != NULL )
    {
        InThreading(root->lchild);
        if( !root->lchild )
        {
            root->Ltag = THREAD;
            root->lchild = pre;

        }
        if(pre != NULL)
        {
            if( !pre->rchild )
            {
                pre->Rtag = THREAD;
                pre->rchild = root;
            }
        }
        pre = root;
        InThreading(root->rchild);
    }
}

void Inthread(BiTree *Thrt , BiTree root)
{
    if(root == NULL)
    {
        return ;
    }
    (*Thrt) = (BiTree)malloc(sizeof(BiNode));
    (*Thrt)->Ltag = LINK;
    (*Thrt)->Rtag = THREAD;
    (*Thrt)->lchild = root;
    (*Thrt)->rchild = (*Thrt);
    pre = (*Thrt);
    InThreading(root);
    pre->Rtag = THREAD;
    (*Thrt)->rchild = pre;
}

BiTree IsNext(BiTree q)
{
    BiTree p = q;
    BiTree next1;
    if(p->Rtag == THREAD)
    {
        next1 = p->rchild;
    }
    else
    {
        p = p->rchild;
        while(p->Ltag == LINK)
        {
            p = p->lchild;
            next1 = p;
        }
    }
    return next1;
}

int Fun(BiTree root11 , BiTree root22)
{
    BiTree root1=root11;
    BiTree root2=root22;
    while(root1->lchild != NULL)
    {
        root1 = root1->lchild;
    }
    while(root2->lchild != NULL)
    {
        root2 = root2->lchild;
    }
    while(root1->data != root2->data)
    {
        root1 = IsNext(root1);
    }
    while(root1->data == root2->data)
    {
        cout << "nextroot1 " << root1->data<< endl;
        cout << "nextroot2 " << root2->data<< endl;

        root1 = IsNext(root1);
        getchar();
        root2 = IsNext(root2);
        getchar();
        /*
        if(root2 == NULL && root1->Rtag == THREAD)
        {
            return 1;
        }
        if(root2 == NULL)
        {
            break;
        }
        */
    }
    return 0;
}

void PreOrder(BiTree root)
{
    if(root != NULL)
    {
        cout << root->data << " ";
        PreOrder(root->lchild);
        PreOrder(root->rchild);
    }
}



int main(int argc,char* argv[])
{
    BiTree root1;
    BiTree Thrt;
    BiTree root2;
    CreatTree(&root1);
    PreOrder(root1);
    InThreading(root1);
    cout << endl;
    CreatTree(&root2);
    PreOrder(root2);
    InThreading(root2);
    cout << endl;
    int ans = Fun(root1,root2);
    cout << ans <<endl;
    return 0;
}
