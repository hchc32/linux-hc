#include <stdio.h>
#include<stdlib.h>
#include<string.h>

int main()
{
    //图的顶点数和边数
    int n,m;
    int j;
    int book[10] = {0};
    int e[10][10];
    int dis[7];
    int inf = 999999999;
    //生成树的节点数
    int count = 0;
    //生成树的权值
    int sum = 0;
    
    scanf("%d %d",&n,&m);

    //初始化
    for(int i = 1 ; i <= n; i++)
    {
        for(int j = 1; j <= n; j++)
        {
            if( i == j )
            {
                e[i][j] = 0;
            }
            else
            {
                e[i][j] = inf;
            }
        }
    }
    
    //读入图的边
    int t1,t2,t3;
    for(int i = 1 ; i <= m; i++)
    {
        scanf("%d %d %d",&t1,&t2,&t3);
        //无向图是对称矩阵
        e[t1][t2] = t3;
        e[t2][t1] = t3;
    }

    //初始化dis数组
    for(int i = 1 ; i <= n ; i++)
    {
        dis[i] = e[1][i];
    }

    //Prim
    //0号顶点加入生成树
    book[1] = 1;
    count++;
    while(count < n)
    {
        int min = inf;
        for(int i = 1 ; i <= n; i++)
        {
            if(book[i] == 0 && dis[i] < min)
            {
                min = dis[i];
                j = i;
            }
        }
        book[j] = 1;
        count++;
        sum = sum + dis[j];

        //扫描当前顶点ｊ所有边,更新dis的值
        for(int k = 1; k <= n ; k++)
        {
            if(book[k] == 0 && dis[k] > e[j][k])
            {
                dis[k] = e[j][k];
            }
        }
    }
    printf("最小生成树的权值为%d\n",sum);
    return 0;


}
