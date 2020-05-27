#include <stdio.h>
#include<stdlib.h>

typedef int VertexType; //顶点类型自定义
//typedef int EdgeType;　//边上权值自定义

//弧结构
typedef struct Arc  
{
    //弧尾,弧头
    char headVex , tailVex;
    //入边表指针域 , 出边表指针域
    struct Arc *headLink;
    struct Arc *tailLink;

}Arc;

//顶点表结点
typedef struct VertexNode 
{
    //顶点数据
    VertexType data;
    //入边表的第一个结点
    Arc *firstin;
    //出边表的第一个结点
    Arc *firstout;
}VertexNode;

//十字链表存储结构
typedef struct
{
    VertexNode List[100];
    //图中顶点数和边数
    int numVertexes , numEdges;
}Graph;

//寻找顶点的下标
int LocalVex(int data , Graph *G)
{
    for(int i = 0; i < G->numVertexes ; i++)
    {
        if(G->List[i].data == data )
        {
            return i;
        }
    }
    return -1;
}

//创建十字链表
void creatOLGraph(Graph *G)
{
    int x,y;
    printf("请输入顶点的个数和边的数量:");
    scanf("%d%d",&G->numVertexes,&G->numEdges);
    for(int i = 0; i < G->numVertexes ; i++)
    {
        printf("输入顶点,建立顶点表:");
        //建立顶点表
        scanf("%d",&G->List[i].data);
        getchar();
        G->List[i].firstin = G->List[i].firstout = NULL;
    }
    /*
    for(int i = 0; i <G->numVertexes; i++)
    {
        printf("%d",G->List[i].data);
    }
    */
    for(int i = 0; i < G->numEdges ; i++)
    {
        getchar();
        printf("读入(vi,vj):");
        scanf("%d,%d",&x,&y);
        int k,j;
        //k是弧尾的位置
        k = LocalVex(x,G);
        //j是弧头的位置
        j = LocalVex(y,G);
        Arc *A = (Arc*)malloc(sizeof(Arc));
        A->tailVex = x;
        A->headVex = y;
        //headLink 弧头的入边
        //tailLink 弧尾的出边
        A->headLink = G->List[j].firstin;
        A->tailLink = G->List[k].firstout;
        G->List[j].firstin = A;
        G->List[k].firstout = A;
    }
}

int main()
{
    Graph G;
    creatOLGraph(&G);
    for(int i = 0; i <G.numVertexes; i++)
    {
        printf("%d\n",G.List[i].data);
    }
    for(int i = 0; i < G.numVertexes ; i++)
    {
        printf("===%d顶点的出度情况为:\n",G.List[i].data);
        Arc *temp = G.List[i].firstout;
        if( !temp )
        {
            printf("无\n");
        }
        while( temp )
        {
            printf("%d --> %d\n",temp->tailVex,temp->headVex);
            temp = temp->tailLink;
        }
        printf("\n");
        printf("===%d顶点的入度情况为:\n",G.List[i].data);
        Arc *temp2 = G.List[i].firstin;
        if( !temp2 )
        {
            printf("空\n");
        }
        while( temp2 )
        {
            printf("%d --> %d\n",temp2->tailVex,temp2->headVex);
            temp2 = temp2->headLink; 
        }
        printf("\n");
    }
    return 0;
}

