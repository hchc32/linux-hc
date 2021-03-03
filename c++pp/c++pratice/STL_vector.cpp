#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

int main()
{
    int ia[6] = {100,2,3,4,5,6};
    //allocator 容器适配器
    //ia ia+6 　迭代器
    //vector    容器
    vector<int,allocator<int>> vi(ia,ia+6);

    //not1　函数适配器
    //
    cout << count_if(vi.begin(),vi.end(),not1(bind2nd(less<int>(),40)));
    return 0;
}

