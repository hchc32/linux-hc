#include <iostream>
#include <vector>
#include <memory>
using namespace std;

void foo_construct()
{
    int *p = new int(3);
    shared_ptr<int> sptr(p);
    cout << sptr.use_count() << endl;
    shared_ptr<int> sptr2(new int(4));
    cout << sptr2.use_count() << endl;
    //cctor
    shared_ptr<int> sptr3(sptr2);
    cout << sptr3.use_count() << endl;
    shared_ptr<int> sptr4 = make_shared<int>(5);
    cout << sptr4.use_count() << endl;
}

int main()
{
    foo_construct();

    return 0;
}