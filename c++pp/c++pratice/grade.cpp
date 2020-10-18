#include <iostream>
#include<string>
#include<vector>


using namespace std;

int main()
{
    int temp = 0;
    vector<int> grade(11,0);
    while( cin >> temp )
    {
        if(temp > 100)
        {
            cout << "The grade is false!" << endl;
        }
        else
        {
            auto it  = grade.begin();
            int n = temp/10;
            it = it + n;
            (*it)++;
        }
    }
    int i = 0;
    cout << "       The score:::   " << endl;
    for(vector<int>::iterator it = grade.begin() ; it != grade.end() ; it++,i++)
    {
        cout << i << ": " << *it << endl;
    }
    return 0;
}
