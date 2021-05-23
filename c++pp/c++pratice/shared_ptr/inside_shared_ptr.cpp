#include <iostream>
#include <vector>
#include <memory>
using namespace std;

class Y : public std::enable_shared_from_this<Y>
{

public:
    Y() = default;
    ~Y() = default;

    std::shared_ptr<Y> GetSharePtr()
    {
        return shared_from_this();
    }

private:
    int x;
};

int main()
{
    //!!error use in stack
    // Y y;
    // std::shared_ptr<Y> spy = y.GetSharePtr();
    
    //!!error use in heap
    //Y* y = new Y;
    //std::shared_ptr<Y> spy1 = y->GetSharePtr();

    std::shared_ptr<Y> spy(new Y);
    std::shared_ptr<Y> p = spy->GetSharePtr();
    cout << p.use_count() << endl;
    return 0;
}