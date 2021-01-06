#include<stdio.h> 
#include<stdlib.h>
typedef struct BiNode{
	int Ltag;
	int Rtag;
	char data;
	struct BiNode *Lchild,*Rchild;
}BiNode,*BiTree;
BiTree pre = NULL;
void Creat_Tree(BiTree *root){
	char ch;
	scanf("%c",&ch);
	if(ch == '#')
		*root = NULL;
	else{
		*root = (BiTree)malloc(sizeof(BiNode));
		(*root)->data = ch;
		//(*root)->Ltag = 0;
		//(*root)->Rtag = 0;
		Creat_Tree(&(*root)->Lchild);
		Creat_Tree(&(*root)->Rchild);
	}
}
void Inorder_Tree(BiTree root){
	if(root == NULL)
		return ;
	else{
		Inorder_Tree(root->Lchild);
		printf("%c ",root->data);
		Inorder_Tree(root->Rchild);
	}
} 
void Inthread(BiTree root){
	if(root != NULL){
		Inthread(root->Lchild);
		if(root->Lchild == NULL){
			root->Lchild = pre;
			root->Ltag = 1;
		}else{
			root->Ltag = 0;
		}
		if(pre != NULL&&pre->Rchild == NULL){
			pre->Rchild = root;
			pre->Rtag = 1;
		}else{
			pre->Rtag = 0;
		}
		pre = root;
		Inthread(root->Rchild);
	}
}
void Prethread(BiTree root){
	if(root != NULL){
		//pre = root;
		//Inthread(root->Lchild);
		if(root->Lchild == NULL){
			root->Lchild = pre;
			root->Ltag = 1;
		}else{
			root->Ltag = 0;
		}
		if(pre != NULL&&pre->Rchild == NULL){
			pre->Rchild = root;
			pre->Rtag = 1;
		}else{
			pre->Rtag = 0;
		}
		pre = root;
		Inthread(root->Lchild);
		Inthread(root->Rchild);
	}
} 
void Inthreading(BiTree *Thrt,BiTree root){
	if(root == NULL)
		return ;
	(*Thrt) = (BiTree)malloc(sizeof(BiNode));
	(*Thrt)->Ltag = 0;
	(*Thrt)->Rtag = 1;
	(*Thrt)->Lchild = root;
	(*Thrt)->Rchild = *Thrt;
	pre = *Thrt;
	Inthread(root);
	pre->Rtag = 1;
	pre->Rchild = *Thrt;
	(*Thrt)->Rchild = pre;
}
void Prethreading(BiTree *Thrt,BiTree root){
	if(root == NULL)
		return ;
	(*Thrt) = (BiTree)malloc(sizeof(BiNode));
	(*Thrt)->Ltag = 0;
	(*Thrt)->Rtag = 1;
	(*Thrt)->Lchild = root;
	(*Thrt)->Rchild = *Thrt;
	pre = *Thrt;
	Prethread(root);
	pre->Rtag = 1;
	pre->Rchild = *Thrt;
	(*Thrt)->Rchild = pre;
}
void InOrderTraverse(BiTree Thrt){
	BiTree p = Thrt->Lchild;
	while (p != Thrt) {
		while (p->Ltag == 0) {
			p = p->Lchild;
		}
		printf("%c", p->data);
	//	printf("%d ",p->Ltag);
	//	printf("%d\n",p->Rtag);
		while (p->Rtag == 1 && p->Rchild != Thrt) {
			p = p->Rchild;
			printf("%c", p->data);
		//	printf("%d ",p->Ltag);
		//	printf("%d\n",p->Rtag);
		}
		p = p->Rchild;
	}
	printf("\n");
}
void PreorderTraverse(BiTree Thrt){
	BiTree p = Thrt->Lchild;
	while(p != Thrt){
		printf("%c ",p->data);
		if(p->Ltag == 0)
			p = p->Lchild;
		else
			p = p->Rchild;
	}
}

/*void TinOrder(BiTree root){
	BiTree p,q;
	p = root;
	if(p == NULL)
		return ;
	while(p->Ltag == 0)
		p = p->Lchild;
	while(p != NULL){
		printf("%d ",p->data);
		if(p->Rtag == 1)
			p = p->Rchild;
		else{
			for(q = p->Rchild;q->Ltag == 0;q = q->Lchild)
				p = q;
		}
	}
}*/
void Inorder_Tree1(BiTree root){
	if(root == NULL)
		return ;
	else{
		Inorder_Tree1(root->Lchild);
		printf("%c ",root->data);
		printf("%d ",root->Ltag);
		printf("%d\n",root->Rtag);
		Inorder_Tree1(root->Rchild);
	}
} 
int main(){
	BiTree root,Thrt;
	Creat_Tree(&root);
	Inthread(root);
	//InOrderTraverse(Thrt);
//	InOrderTraverse(Thrt);
//	Inorder_Tree1(root);
//	printf("%c ",root->data);
//	Inthreading(&Thrt,root);
//	InOrderTraverse(Thrt);
//	printf("%c ",root->data);
	//TinOrder(root);
	//Inorder_Tree(root);
	return 0;
}
