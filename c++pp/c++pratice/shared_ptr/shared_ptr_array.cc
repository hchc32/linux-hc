#include<iostream>
#include<memory>
using namespace std;

void del(int *p)
{
    delete[] p;
}

int main()
{
    int *p = new int[100];
    for (int i = 0;i<100;i++)
    {
        p[i] = i;
    }
    shared_ptr<int> shared1(p, del);
    shared_ptr<int> shared2(new int[100], [](int *p) {
        delete[] p;
    });
    int n = 100;
    for (int i=0;i<n;i++)
    {
        cout << *shared1.get()+i << endl;
    }
    return 0;
}