#include <iostream>
#include<vector>
using namespace std;

int main()
{
    string a = "hello\n";
    vector<int> aa = {1,2,3};
    cout << &aa[2] << endl;

    vector<vector<int>> vec;
    vec.push_back(std::move(aa));

    cout << vec[0][2]  << endl;
    cout << &vec[0][2] << endl;
    cout << aa.size()  << endl;
    return 0;
}

