#include <iostream>
#include<string>

using namespace std;

class Point
{
private:
    int i,j,k;
public:
    Point() { i=j=k=0; }
    Point( int ii, int jj, int kk) { i=ii , j=jj , k=kk; }
    void print(string& msg)
    {
        if(msg.size() != 0)
            cout << msg << endl;
        cout << "i = " << i << endl;
        cout << "j = " << j << endl;
        cout << "k = " << k<< endl;
    }
};
int main()
{
    Point p,q(1,2,3);
    p.print("value of p");
    q.print("value of q");

    return 0;
}
