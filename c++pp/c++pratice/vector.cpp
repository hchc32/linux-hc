#include <iostream>
#include<vector>

using std::vector;
int main()
{
    vector<int> str;
    str.push_back(6);
    str.push_back(5);
    std::cout << "size = " << str.size() << std::endl;
    for(auto &c : str)
    {
        c = c * c;
    }
    for(auto &c : str)
    {
        std::cout << c << std::endl;
    }
    return 0;
}
