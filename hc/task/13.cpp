#include <iostream>
#include<queue>

using namespace std;

//地图
int map[6][6]
{
    { 0, 0, 0, 1, 0, 0 },
    { 0, 1, 0, 0, 1, 0 },
	{ 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 1, 0, 1, 1 },
	{ 0, 0, 1, 0, 0, 0 },
};

//标记路径
int visit[50][50];
//方向数组
int Next[4][2] = 
{
    {0,1},
    {1,0},
    {0,-1},
    {-1,0}
};

int n,m;

struct node
{
    int x;
    int y;
    int step;
    string str;
    node(int xx,int yy,int ss,string sstr)
    {
        x = xx;
        y = yy;
        step = ss;
        str=sstr;
    }
};

queue<node> que;


int NotGo(int x,int y)
{
    if(x < 0 || y < 0 || x > n || y > m || visit[x][y] == 1 || map[x][y] == 1)
    {
        return 0;
    }
    return 1;
}


void bfs(int x,int y)
{
        que.push(node(x,y,0,""));
        visit[x][y] = 0;
        while(!que.empty())
        {
            node temp = que.front();        
            if(temp.x == n && temp.y == m)
            {
                cout << "^" <<  temp.step << endl;
                break;
            }
            que.pop();
            for(int i = 0 ; i < 4 ; i++)
            {
                int temp_x = temp.x + Next[i][0];
                int temp_y = temp.y + Next[i][1];
                if(NotGo(temp_x,temp_y))
                {
                    que.push(node(temp_x,temp_y,temp.step+1,""));
                    visit[temp_x][temp_y] = 1;
                }
            }
            
        }
}

int main()
{
    scanf("%d%d",&n,&m);
    bfs(0,0);
    return 0;
}
