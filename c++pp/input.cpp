#include <iostream>

using namespace std;

int main()
{
    int sum = 0 , value = 0;

    //读取数据直到文件末尾
    while(std::cin >> value)
    {
        sum += value;
    }
    cout << "The sum is " << sum << endl;

    return 0;
}
