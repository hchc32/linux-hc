#include<iostream>
#include<string>
#include<unordered_set>
#include<vector>
using namespace std;
int main()
{
    while(1)
    {
        unordered_set<string> s;
        vector<string> array;
        string temp;
        int n;
        string answer;
        cin >> n;
        if(n == 0)
            return 0;
        for(int i = 0; i < n; i++)
        {
            cin >> temp;
            s.insert(temp);
            array.push_back(temp);
        }
        int max = 0;
        for(auto it = s.begin() ; it!=s.end() ; it++)
        {
            int count = 0;
            for(auto i = array.begin(); i!=array.end(); i++)
            {
                if((*it) == (*i))
                {
                    count++;
                }
            }
            if(max < count)
            {
                max = count;
                answer = (*it);
            }
        }
        cout << answer << endl;
    }
    return 0;
}
