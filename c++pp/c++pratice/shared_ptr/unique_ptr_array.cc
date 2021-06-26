#include<iostream>
#include<memory>
using namespace std;


int main()
{
    unique_ptr<int []> shared(new int[100]);
    [&shared]() {
        for (int i = 0;i<100;i++)
        {
            shared[i] = i;
        }
    }();
    for(int i = 0;i<100;i++)
    {
        cout << shared[i] << endl;
    }

    return 0;
}