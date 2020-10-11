#include<iostream>
#include<stack>

typedef struct Pos
{
    int x;
    int y;
}Pos;

//迷宫地图
int map[9][8] =
{
    0,0,1,0,0,0,1,0,
    0,0,1,0,0,0,1,0,
    0,0,0,0,1,1,0,1,
    0,1,1,1,0,0,1,0,
    0,0,0,1,0,0,0,0,
    0,1,0,0,0,1,0,1,
    0,1,1,1,1,0,0,1,
    1,1,0,0,0,1,0,1,
    1,1,0,0,0,0,0,0,
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


void migong(Pos start)
{
    //起始坐标
    if(start.x > 8 && start.y > 8 && start.x < 0 && start.y < 0)        
    {
        return;
    }

    //判断坐标是否合法
    //如果是终点,直接break;
    //  如果合法,入栈,继续寻找下一步
    //      将下一步所有可以走的点入栈
    //      依次取栈顶尝试是否可以走通
    //  否则出栈
}


int main()
{
    
    return 0;
}
