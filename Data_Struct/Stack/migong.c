#include<stdio.h>
#include<stdlib.h>
#include"Stack.h"


//迷宫地图
int map[9][8] = 
{   {0,0,1,0,0,0,1,0},
    {0,0,1,0,0,0,1,0},
    {0,0,0,0,1,1,0,1},
    {0,1,1,1,0,0,1,0},
    {0,0,0,1,0,0,0,0},
    {0,1,0,0,0,1,0,1},
    {0,1,1,1,1,0,0,1},
    {1,1,0,0,0,1,0,1},
    {1,1,0,0,0,0,0,0},
};

//移动方向
int fangxiang1[4][2] = 
{
    //右1
    {0,1},
    //下2
    {1,0},
    //左3
    {0,-1},
    //上4
    {-1,0},
};

void dfs()
{
    //初始化栈
    LinkStack* head = Init_Stack();
    Pos start;
    start.x = 0;
    start.y = 0;
    start.fx = -1;
    Push(head,start);
    while(!Is_Empty(head))
    {
        Pos temp1;
        Get_Top(head,&temp1);
        //非法坐标
        if(start.x > 7 || start.y > 8 || start.x < 0 || start.y < 0)
        {
            break;
        }
        for(int i = 0; i < 4; i++)
        {
            Pos temp;
            int x = temp1.x + fangxiang1[i][0];
            int y = temp1.y + fangxiang1[i][1];
            if(x <= 7 && y <=8 && x >= 0 && y >= 0)
            {
                if(map[x][y] == 0)
                {
                    map[x][y] = 2;
                    //head->top->data.fx = i+1; 
                    getchar();
                    temp.x = x;
                    temp.y = y;
                    temp.fx = -1;
                    printf("%d %d %d\n",temp.x,temp.y,temp.fx);
                    Push(head,temp);
                    break;
                }
                if(i == 3)
                {
                    Pos del;
                    Pop(head,&del);
                    printf("pop : %d %d %d\n",del.x,del.y,del.fx);
                    break;
                }

            }
            if(temp1.x == 7 && temp1.y == 8)
            {
                break;
            }
        }
        //到达终点
    }
    while(!Is_Empty(head))
    {
        Pos temp;
        Pop(head,&temp);
        printf("%d %d %d\n",temp.x,temp.y,temp.fx);
    }
}

int main()
{
    dfs();
    return 0;
}
