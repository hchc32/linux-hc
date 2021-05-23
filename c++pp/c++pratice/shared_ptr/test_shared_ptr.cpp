//shared_ptr作为被保护对象的成员时，小心循环引用造成无法释放资源
//可以用weak_ptr解决该问题
//
#include<iostream>
#include<memory>
using namespace std;

class CB;
class CA;


void PRINT_FUN()
{
    cout << "bye" << endl;
}

class CA
{
public:
    CA() {}
    ~CA() { PRINT_FUN(); }

    void Register(const std::shared_ptr<CB> &sp)
    {
        m_sp = sp;
    }

private:
    weak_ptr<CB> m_sp;
    //std::shared_ptr<CB> m_sp;
};

class CB
{
public:
    CB(){};
    ~CB() { PRINT_FUN(); };

    void Register(const std::shared_ptr<CA> &sp)
    {
        m_sp = sp;
    }

private:
    //weak_ptr<CA> m_sp;
    std::shared_ptr<CA> m_sp;
};
int main()
{
    int *p1;
    //8
    cout << sizeof(p1) << endl;
    std::shared_ptr<CA>
        spa(new CA);
    std::shared_ptr<CB> spb(new CB);
    spb->Register(spa);
    spa->Register(spb);
    printf("%d\n", spb.use_count()); // 2
    printf("%d\n", spa.use_count()); // 2

}

