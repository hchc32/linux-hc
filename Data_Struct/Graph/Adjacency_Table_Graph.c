#include <stdio.h>
#include<stdlib.h>

typedef char VertexType; //顶点类型自定义 
typedef int  EdgeType; //边上的权值类型自定义
#define MAXVEX 100
//边表结点
typedef struct EdgeNode
{
    VertexType adjvex; //存储顶点
    EdgeType weight; //存储权值
    struct EdgeNode *next; //指向下一个邻接点
}EdgeNode;

//顶点表结点
typedef struct VertexNode
{
    VertexType data; //存储顶点信息
    EdgeNode *firstedge; //边表头指针
}VertexNode,AdjList[MAXVEX];

//图的邻接表存储结构
typedef struct 
{
    //结构体数组
    AdjList adjList;
    int numVertexes,numEdges; //图中的顶点数和边数
}GraphAdjList;


//建立无向图的邻接表
void creatALGraph(GraphAdjList *G)
{
    int i,j,k;
    printf("输入顶点数和边数:\n");
    scanf("%d,%d",&G->numVertexes,&G->numEdges);
    //建立顶点表
    printf("%d,%d",G->numVertexes,G->numEdges);
    getchar();
    for(i = 0 ;i < G->numVertexes ; i++)
    {
        //输入顶点信息
        printf("请输入顶点信息:");
        scanf("%c",&G->adjList[i].data);
        //将顶点对应的边表置空
        G->adjList->firstedge = NULL;    
        getchar();
    }
    //建立边表
    for(k = 0; k < G->numEdges ; k++)
    {
        printf("输入边(vi,vj)上的顶点序号:\n");
        scanf("%d,%d",&i,&j);
        //头插法
        EdgeNode *e1 = (EdgeNode*)malloc(sizeof(EdgeNode));
        e1->adjvex = j; //邻接序号为ｊ
        e1->next = G->adjList->firstedge;
        G->adjList->firstedge = e1;

        EdgeNode *e2 = (EdgeNode*)malloc(sizeof(EdgeNode));
        e2->adjvex = i; //邻接序号为i
        e2->next = G->adjList->firstedge;
        G->adjList->firstedge = e2;
    }
}

int main()
{   
    GraphAdjList G;
    creatALGraph(&G);
    return 0;
}

