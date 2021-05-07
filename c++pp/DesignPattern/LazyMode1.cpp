#include <iostream>
#include <mutex>
using namespace std;
class Singleton
{
public:
    Singleton *GetInsTance()
    {
        if(m_PInstance==nullptr)
        {
            m_tex.lock();
            if(m_PInstance == nullptr)
            {
                m_PInstance = new Singleton();
            }
            m_tex.unlock();
        }
        return m_PInstance;
    }
    class CGarbo
    {
        ~CGarbo()
        {
            if(m_PInstance != nullptr)
            {
                delete  Singleton::m_PInstance;
                Singleton::m_PInstance = nullptr;
            }
        }
    };
    //定义一个静态变量,程序结束时,系统会自动调用他的析构函数.
    static CGarbo cgarbo;
private:
    //私有构造函数
    Singleton() {}
    //拷贝构造函数
    Singleton(Singleton const&) = delete;
    //拷贝赋值函数
    Singleton& operator=(Singleton &) = delete;

private:
    static Singleton* m_PInstance;
    static mutex m_tex;
};

//静态局部变量，避免自己回收资源
class Singleton
{
public:
    static Singleton* GetInsTance()
    {
        static Singleton instance;
        return &instance;
    }
private:
    Singleton() {};
    Singleton(const Singleton&) = delete;
    Singleton& operator = (const Singleton &) = delete;
};


//饿汉模式一
//初始化顺序不定可能会出错
class Singleton
{
public:
    static Singleton* GetInsTance()
    {
        return &m_instance;
    }
private:
    Singleton() {};
    Singleton(Singleton const&);
    Singleton& operator= (Singleton const&);

    static Singleton m_instance;
};

//饿汉模式二(boost实现)
//用辅助类
class Singleton
{
public:
    static Singleton* GetInsTance()
    {
        static Singleton instance;
        return &instance;
    }

protected:
    //辅助代理类
    struct Object_Creator
    {
        Object_Creator()
        {
            Singleton::GetInsTance();
        }
    }
    static Object_Creator _object_creator;
    Singleton() {};
    ~Singleton() {};
};
