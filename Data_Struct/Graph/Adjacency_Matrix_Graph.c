//创建无向网图
#include <stdio.h>
#include<stdlib.h>

//顶点类型
typedef char VertexType;
//边上的权值类型
typedef int EdgeType;

#define MAXVER 100

#define INFINITY 65535

//图的邻接矩阵存储结构
typedef struct
{
    VertexType vexs[MAXVER]; //顶点表
    EdgeType arc[MAXVER][MAXVER]; //邻接矩阵,又称为边表
    int numVertexes , numEdges; //图中当前的顶点数和边数
}MGraph;

//创建无向网图邻接矩阵
//时间复杂度O(n+n*2+e)
void creatMGraph(MGraph *G)
{
    printf("输入顶点数和边数:\n");
    scanf("%d%d",&G->numVertexes,&G->numEdges);
    for(int i = 0; i < G->numVertexes; i++)
    {
        printf("请输入顶点信息:\n");
        scanf("%c",&G->vexs[i]);
        getchar();
    }
    for(int i = 0; i < G->numVertexes; i++)
    {
        for(int j = 0; j < G->numVertexes; j++)
        {
            G->arc[i][j] = INFINITY;
        }
    }
    for(int k = 0; k < G->numEdges; k++)
    {
        int i,j,w;
        printf("输入边(vi,vj)上的下标i , 下标j和权w:\n");
        scanf("%d,%d,%d",&i,&j,&w);
        G->arc[i][j] = w;
        //无向图是对称矩阵
        G->arc[j][i] = G->arc[i][j];
    }
}

int main()
{
    MGraph G;
    creatMGraph(&G);
    return 0;
}

