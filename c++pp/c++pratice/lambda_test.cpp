#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int main()
{
    vector<int> a{1,2,3,4,5};
    
    for(auto x : a)
    {
        cout << x << " ";
    }
    cout << endl;

    cout << "start....." << endl;
    sort(a.begin(),a.end(),[=](int x,int y)->bool {
         cout << x << " " << y << endl;
         return x%3 < y%3;
         });

    cout << "end....." << endl;
    int sz=3;
    cout << "start....." << endl;
    auto p = find_if(a.begin(),a.end(),[sz](const int &a){
            cout << a << endl;
            return a>sz;
            });
    cout << *p;
    cout << "end....." << endl;
    for(auto x1 : a)
    {
        cout << x1 << " ";
    }
    cout << endl;

    return 0;
}

