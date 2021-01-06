#include <stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE 1000
typedef struct node 
{
    int weight;
    int parent,lchild,rchild;
}HTNode,HuffmanTree[MAX_SIZE+1];

int w[MAX_SIZE] = {99999,5,7,3,2,8};


void select1(HuffmanTree ht , int n , int *s1 , int *s2)
{
    int min = 99999;
    int j = -1;
    int i;
    for(i = 1; i <= n ;i++)
    {
        if(ht[i].parent == 0 && min > ht[i].weight)
        {
            min = ht[i].weight;
            j=i;
        }
    }
    *s1 = j;
    min=99999;
    for(i = 1; i <= n ;i++)
    {
        if(j!=i && ht[i].parent == 0 && min > ht[i].weight)
        {
            min = ht[i].weight;
            j=i;
        }
    }
    *s2 = j ;
}

//w为权值数组
//n为叶子结点个数
void CreatHuffmanTree(HuffmanTree ht , int w[] , int n)
{
    //结点总个数
    int m = 2*n-1;
    int s1,s2;
    //初始化前ｎ个结点
    for(int i = 1; i <= n ;i++)
    {
        ht[i].weight = w[i];
        ht[i].lchild = 0;
        ht[i].rchild = 0;
        ht[i].parent = 0;
    }

    for(int i = n+1 ; i<=m ; i++)
    {
        ht[i].weight = 0;
        ht[i].lchild = 0;
        ht[i].rchild = 0;
        ht[i].parent = 0;
    }
    for(int i = n+1 ; i<=m ; i++)
    {
        select1(ht,i-1,&s1,&s2);
        printf("s1 = %d s2 = %d\n",s1,s2);
        s1=4,s2=3;
        ht[i].weight = ht[s1].weight  + ht[s2].weight;
        ht[i].lchild = s1;
        ht[i].rchild = s2;
        ht[s1].parent = i;
        ht[s2].parent = i;
    }
}

int main()
{

    HuffmanTree ht;
    int n = 5;
    CreatHuffmanTree(ht,w,n);
    for(int i = 1 ; i <= n; i++)
    {
        printf("%d %d %d %d\n",ht[i].weight,ht[i].parent,ht[i].lchild,ht[i].rchild);
    }
    return 0;
}



