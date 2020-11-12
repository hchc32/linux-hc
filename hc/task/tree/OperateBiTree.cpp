#include <iostream>
#include<cstring>
#include<cstdlib>
#include<stack>

using namespace std;

#define MAXSIZE 1000
typedef struct node
{
    char key;
    struct node *lchild;
    struct node *rchild;
}BTNode;

//判断运算符号优先级
int GetPriority(char c)
{
    if(c == '(')
    {
        return 1;
    }
    else if(c == '+' || c== '-')
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

//判断是否是操作符号
int IsOperate(char c)
{
    if(c == '+' || c == '-' || c == '*' || c == '/' ||c == '(' || c == ')')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// 中序遍历二叉树
void InOrderTraveres (BTNode* T)
{
    if(T)
    {
        InOrderTraveres (T->lchild);
        cout<<T->key<< " ";
        InOrderTraveres (T->rchild);
    }
}

//根据中缀表达式建立表达式树
void InorderCreatOTree(char *str , BTNode *& root)
{
    stack<char> Operator_Stack;
    stack<BTNode*> Node_Stack;
    BTNode* node;
    char *temp = str;
    while(*temp != '\0')
    {
        //是操作符号
        if(IsOperate(*temp)) 
        {
            if(*temp == '(')
            {
                Operator_Stack.push(*temp);
                temp++;
                continue;
            }
            if(*temp == ')')
            {
                while(Operator_Stack.top() != '(')
                {
                    node = (BTNode*)malloc(sizeof(BTNode));
                    node->key = Operator_Stack.top();
                    Operator_Stack.pop();
                    node->rchild = Node_Stack.top();
                    Node_Stack.pop();
                    node->lchild = Node_Stack.top();
                    Node_Stack.pop();
                    Node_Stack.push(node);
                }
                Operator_Stack.pop();
                temp++;
                continue;
            }
            if(!Operator_Stack.empty() && GetPriority(*temp) < GetPriority(Operator_Stack.top()))
            {
                node = (BTNode*)malloc(sizeof(BTNode));
                node->key = Operator_Stack.top();
                Operator_Stack.pop();
                node->rchild = Node_Stack.top();
                Node_Stack.pop();
                node->lchild = Node_Stack.top();
                Node_Stack.pop();
                Node_Stack.push(node);
                Operator_Stack.push(*temp);
            }
            else
            {
                Operator_Stack.push(*temp);
            }
        }
        //是操作数
        else
        {
            node = (BTNode*)malloc(sizeof(BTNode));
            node->key = *temp;
            node->lchild = NULL;
            node->rchild = NULL;
            Node_Stack.push(node);
        }
        temp++;
    }
    //当操作数栈不为空时
    while(!Operator_Stack.empty())
    {
         if(Operator_Stack.top() == ')')
         {
             while(Operator_Stack.top() != '(')
             {
                 Operator_Stack.pop();
                 node = (BTNode*)malloc(sizeof(BTNode));
                 node->key = Operator_Stack.top();
                 Operator_Stack.pop();
                 node->rchild = Node_Stack.top();
                 Node_Stack.pop();
                 node->lchild = Node_Stack.top();
                 Node_Stack.pop();
                 Node_Stack.push(node);
             }
             Operator_Stack.pop();
             continue;
         }
        node = (BTNode*)malloc(sizeof(BTNode));
        node->key = Operator_Stack.top();
        Operator_Stack.pop();
        if(!Node_Stack.empty())
            node->rchild = Node_Stack.top();
        Node_Stack.pop();

        if(!Node_Stack.empty())
            node->lchild = Node_Stack.top();
        Node_Stack.pop();
        Node_Stack.push(node);
    }
    root = Node_Stack.top();
}


// 前序遍历二叉树
void PreOrderTraveres (BTNode* T)
{
    if(T)
    {
        cout<<T->key<<" ";
        PreOrderTraveres (T->lchild);
        PreOrderTraveres (T->rchild);
    }
}

// 后序遍历二叉树
void PostOrderTraveres (BTNode* T)
{
    if(T == NULL)
        return ;
    PostOrderTraveres (T->lchild);
    PostOrderTraveres (T->rchild);
    cout<<T->key<<" ";
}

int GetValue(char oper,char key1,char key2)
{
    int data1 = key1-48;
    int data2 = key2-48;
    cout << "data1 = " << data1<<endl;
    cout << "data2 = " << data2<<endl;
    switch(oper)
    {
        case '+':{
                    return data1+data2;
                 }
        case '-':{
                    return data1-data2;
                 }
        case '*':{
                    return data1*data2;
                 }
        case '/':{
                    return data1/data2;
                 }
    }
    return -1;
}




static int Calculate(BTNode* T)
{
    int oper1=0,oper2=0;
    //是叶子结点
    if(T == NULL)
    {
        return 0;
    }
    if(T->lchild == NULL && T->rchild == NULL)
    {
        return T->key-'0';
    }
    oper1 = Calculate(T->lchild);
    oper2 = Calculate(T->rchild);
    switch(T->key)
    {
         case '+':{
                     return oper1+oper2;
                  }
         case '-':{
                     return oper1-oper2;
                  }
         case '*':{
                     return oper1*oper2;
                  }
         case '/':{
                     return oper1/oper2;
                  }
         default:{
                     exit(0);
                 }
    }
}



int main()
{
    BTNode *root;
    char str[MAXSIZE];
    cin >> str;
    InorderCreatOTree(str,root);
    /*
    cout<<root->key<<endl;
    cout<<root->lchild->key<<endl;
    cout<<root->rchild->key<<endl;
    cout<<root->lchild->lchild->key<<endl;
    cout<<root->lchild->rchild->key<<endl;
    cout<<root->rchild->lchild->key<<endl;
    cout<<root->rchild->rchild->key<<endl;
    cout<<root->rchild->lchild->lchild->key<<endl;
    cout<<root->rchild->lchild->rchild->key<<endl;
    cout<<root->rchild->rchild->lchild->key<<endl;
    cout<<root->rchild->rchild->rchild->key<<endl;
    */
    cout<<"前序遍历:"<<endl;
    PreOrderTraveres(root);
    cout<<endl;
    cout<<"中序遍历:"<<endl;
    InOrderTraveres(root);
    cout<<endl;
    cout<<"后序遍历:"<<endl;
    PostOrderTraveres(root);
    cout << Calculate(root) <<endl;
    return 0;
}
