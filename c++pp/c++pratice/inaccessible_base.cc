#include<iostream>

using namespace std;

namespace NS
{
    class Quote
    {
    };

    void display(const Quote &) { cout << "linux\n"; }

} // namespace NS

class Bulk : public NS::Quote
{
};

int main()
{
    Bulk book1;
    display(book1);
    return 0;
}