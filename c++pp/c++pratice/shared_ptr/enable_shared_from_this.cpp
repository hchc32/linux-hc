//如何在类对象内部获得一个指向当前对象的shared_ptr对象 ---->enable_shared_from_this()
#include <iostream>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
class Test : public boost::enable_shared_from_this<Test>        //改进1
{
public:
    //析构函数
    ~Test() { std::cout << "Test Destructor." << std::endl; }
    //获取指向当前对象的指针
    boost::shared_ptr<Test> GetObject()
    {
        return shared_from_this();      //改进2
    }
};
int main(int argc, char *argv[])
{
  //  {
        boost::shared_ptr<Test> p( new Test( ));
        boost::shared_ptr<Test> q = p->GetObject();
        std::cout << "q.use_count(): " << q.use_count() << std::endl;
  //  }

    return 0;
}