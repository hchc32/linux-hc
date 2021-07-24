#include<iostream>
#include<set>

using namespace std;

struct formation
{
    formation(string n,int num)
    {
        name=n;
        number = num;
    }
    /*
    bool friend operator<(node x,node y)
    {
    	if(x.a<y.a) return true;
    	if(x.a==y.a&&x.b<y.b) return true;
    	if(x.a==y.a&&x.b==y.b&&x.c<y.c) return true;
    	if(x.a==y.a&&x.b==y.b&&x.c==y.c&&x.d<y.d) return true;
    	return false;
    */
    bool friend operator<(const formation &a2,const formation& a1)
    {
        if(a1.name < a2.name && a2.number < a1.number)
        {
            return true;
        }
        return false;
    }

  string name;
  int number;
};
set<formation> ans;
int main()
{
    string n;
    int num;
    int count=0;
    while(count!=2)
    {
        while(1)
        {
            cin >> n;
            if(n == "end")
            {
                count++;
                break;
            }
            cin >> num;
            ans.insert(formation(n,num));     
        }
    }
    for(auto &x : ans)
    {
        cout << (x).name << " " << (x).number << endl;
    }
    return 0;
}