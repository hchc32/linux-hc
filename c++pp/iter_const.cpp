#include <iostream>
#include<string>
#include<vector>
using namespace std;

int print_int_vector(vector<int> a)
{
    //注意size的值不是int,要注意类型
    for(vector<int>::size_type i = 0 ; i != a.size() ; i++)
    {
        cout << a[i] << " ";
    }
    cout << endl;
    return 1;
}

int main()
{
    vector<int> a(10,68);
    print_int_vector(a);
    //迭代器
    for(vector<int>::iterator it = a.begin() ; it != a.end() ; ++it)
    {
        *it = rand()%100;
    }
    print_int_vector(a);
    return 0;
}
