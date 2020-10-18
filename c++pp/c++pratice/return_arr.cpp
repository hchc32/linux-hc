#include<iostream>
#include<cstdlib>
#include<ctime>


using namespace std;
int* getrandom()
{
    //c++不支持在函数外部返回局部变量的地址，除非定义为static
    //ｃ也不支持
    static int r[10];
    //设置种子
    srand(time(NULL));
    for(int i = 0; i < 10 ; i++)
    {
        r[i] = rand();
        cout << r[i] << endl;
    }
    return r;
}

int main()
{
    int* p = getrandom();
    for(int i = 0; i < 10 ;i++)
    {
        cout << "*(p + " << i <<"):";
        cout << *(p+i) <<endl;
    }

    return 0;
}
