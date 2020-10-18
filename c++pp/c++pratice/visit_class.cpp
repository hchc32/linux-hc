#include <iostream>
using namespace std;

class Box
{
    public:
        double length; //长度
        double breadth; //宽度
        double height; //高度

};

int main()
{
    //声明Box1 和 Box2 ,类型为Box
    Box Box1;
    Box Box2;
    //体积
    double  volume = 0.0;

    Box1.height = 5.0;
    Box1.length = 6.0;
    Box1.breadth = 7.0;


    Box2.height = 10.0;
    Box2.length = 12.0;
    Box2.breadth = 13.0;

    volume = Box1.height * Box1.length * Box1.breadth;

    cout << "Box1 的体积: "<< volume << endl;
    
    volume = Box2.height * Box2.length * Box2.breadth;
    
    cout << "Box2 的体积: "<< volume << endl;
    return 0;
}
