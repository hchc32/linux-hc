#include<iostream>
#include<cstdlib>
#include<cstdio>
#define MAXN 128
using namespace std;

typedef struct BiNode{
	char data;
	struct BiNode *Lchild,*Rchild;
}BiNode,*BiTree;

typedef struct queue{
	BiNode *data[MAXN];
	int front;
	int rear;
}SeqQueue;
SeqQueue q;
int Empty_Queue(){
	if(q.front == q.rear)
		return 1;
	else
		return 0;

}
void Init_Queue(){
	q.front = -1;
	q.rear = -1;
}
void Push_Queue(BiTree root){
	if(q.rear == MAXN-1)
		return ;
	else{
		q.data[q.rear++] = root;
	}
}
BiTree Pop_Queue(){
	if(!Empty_Queue())
	{
		return (q.data[q.front++]);
    }
}

void Creat_Tree(BiTree *root){
	char ch;
	scanf("%c",&ch);
	if(ch == '#')
		*root = NULL;
	else{
		*root = (BiTree)malloc(sizeof(BiNode));
		(*root)->data = ch;
		Creat_Tree(&(*root)->Lchild);
		Creat_Tree(&(*root)->Rchild);
	}
}
void LevelOrder_Tree(BiTree root){
	BiTree temp;
	Push_Queue(root);
	while(!Empty_Queue()){
		temp = Pop_Queue();
		printf("%c ",temp->data);
		if(temp->Lchild)
			Push_Queue(temp->Lchild);
		if(temp->Rchild)
			Push_Queue(temp->Rchild);
	}
}
bool Same_Tree( BiTree p,BiTree q){
	if(p == NULL&&q == NULL)
		return true;
	if(p == NULL&&q != NULL)
		return false;
    if(p != NULL&&q == NULL)
        return false;
	if(p->data != q->data)
		return false;
	return Same_Tree(p->Lchild,q->Lchild)&&Same_Tree(p->Rchild,q->Rchild);
}
bool Sub_Tree(BiTree p, BiTree q){
	if(p == NULL&&q == NULL)
		return true;
	if(p == NULL&&q != NULL)
		return false;
    if(p != NULL&&q == NULL)
        return false;
	return Same_Tree(p,q)||Sub_Tree(p->Lchild,q)||Sub_Tree(p->Rchild,q);

}
void Mirror_Tree(BiTree *root){
    if(*root != NULL){
        BiTree temp;
        temp = (*root)->Lchild;
        (*root)->Lchild = (*root)->Rchild;
        (*root)->Rchild = temp;
    }
    Mirror_Tree(&(*root)->Lchild);
    Mirror_Tree(&(*root)->Rchild);
}
BiTree Mirror_Tree1(BiTree root){
    if(root == NULL)
        return NULL;
    else{
        BiTree temp;
        temp = root->Lchild;
        //root->Lchild = root->Rchild;
        //root->Rchild = temp;
        root->Lchild = Mirror_Tree1(root->Rchild);
        root->Rchild = Mirror_Tree1(temp);
    }
    return root;
}

void print(BiTree root)
{
    if(root!=NULL)
    {
        cout <<root->data << endl;
        print(root->Lchild);
        print(root->Rchild);
    }
}
int main(){
	BiTree root;
	BiTree root1;
	printf("请输入主树：");
	Creat_Tree(&root);
    print(root);
	//Mirror_Tree1(root);
	//LevelOrder_Tree(root);
	printf("请输入要判断的子树：");
    getchar();
	Creat_Tree(&root1);
    
    print(root1);
	cout<<boolalpha<<Sub_Tree(root,root1);
	return 0;
}
