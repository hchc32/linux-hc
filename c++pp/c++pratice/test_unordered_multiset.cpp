/*
 * 1000000
 * test_unordered_set start.............
 * unordered_set.size() = 0
 * milli-seconds : 674424
 * unordered_set.max_size() = 384307168202282325
 * unordered.size() = 998385insert 
 * end .........
 * ::find() milli-seconds : 89815
 * c.find() milli-seconds : 1
 */
#include <iostream>
#include <unordered_set>
#include <algorithm>
using namespace std;
namespace umstest
{
void test_unordered(long& value)
{
    unordered_set<string> c;
    char buf[10];
    cout << "test_unordered_set start............." << endl;
    cout << "unordered_set.size() = " << c.size() <<endl;
    clock_t timeStart = clock();
    for(long i = 0 ; i < value ; ++i)
    {
        try
        {
            snprintf(buf,10,"%d",rand());
            c.insert((string)buf);
        }
        catch(exception& p)
        {
            cout << "i=" << i << " " <<p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds : " << (clock()-timeStart) << endl;
    cout << "unordered_set.max_size() = " << c.max_size() << endl;
    cout << "unordered.size() = " << c.size() <<endl;
    cout << "insert end ........." << endl;

    timeStart = clock();
    snprintf(buf,10,"%d",rand());
    string find_value = (string)buf;
    ::find(c.cbegin(),c.cend(),find_value);
    cout << "::find() milli-seconds : " << (clock()-timeStart) << endl;
    timeStart = clock();
    c.find(find_value);
    cout << "c.find() milli-seconds : " << (clock()-timeStart) << endl;
}
}
int main()
{
    long value;
    cin >> value;
    umstest::test_unordered(value);
    return 0;
}

