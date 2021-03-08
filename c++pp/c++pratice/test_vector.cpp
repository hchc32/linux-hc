#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

namespace vtest
{
void test_vector(long& value)
{
    vector<string> c;
    char buf[10];
    cout << "test_vector start............." << endl;
    cout << "vector.size() = " << c.size() <<endl;
    clock_t timeStart = clock();
    for(long i = 0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand());
            c.push_back((string)buf);
        }
        catch(exception& p)
        {
            cout << "i=" << i << " " <<p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds : " << (clock()-timeStart) << endl;
    cout << "vector.max_size() = " << c.max_size() << endl;
    cout << "vector.size() = " << c.size() <<endl;
    cout << "vector.capacity() = " << c.capacity() << endl;
}
}
int main()
{
    long value;
    cin >> value;
    vtest::test_vector(value);
    return 0;
}

