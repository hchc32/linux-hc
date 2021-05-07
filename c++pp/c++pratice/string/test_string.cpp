#include<iostream>
#include<cstdlib>
using namespace std;

//智能指针
template<typename T>
class Share_Ptr
{
private:
    size_t *Ptr_Count; //引用计数
    T*      Mem_Ptr;   //原始指针

public:
    Share_Ptr():Ptr_Count(nullptr),Mem_Ptr(nullptr) {}; //默认构造函数

    //防止内部隐性转换
    explicit Share_Ptr(T* tmp_Ptr):Mem_Ptr(tmp_Ptr),Ptr_Count(nullptr) {};

    //不同类型智能指针的转换
    template<class type>
    Share_Ptr(Share_Ptr <T>& ptr):Mem_Ptr(ptr->get()),Ptr_Count(++(ptr->use_count)) {};
    
    //copy ctor
    Share_Ptr(Share_Ptr &tmp_ptr)
    {
        Mem_Ptr = tmp_ptr.Mem_Ptr;
        Ptr_Count = tmp_ptr.Ptr_Count;
        ++*Ptr_Count;
    }


    void operator=(const Share_Ptr & tmp_ptr)
    {
        //将tmp_ptr变为亡值,避免内存中不必要的copy
        Share_Ptr(std::move(tmp_ptr));
    }

    //不同类型的拷贝复制
    template<class type>
    void operator=(const Share_Ptr<type> & tmp_ptr)
    {
        Ptr_Count = ++(tmp_ptr.use_count());
        Mem_Ptr = tmp_ptr.get();
    }
    //move ctor
    Share_Ptr(Share_Ptr && tmp_ptr):Mem_Ptr(tmp_ptr.Mem_Ptr),Ptr_Count(tmp_ptr.Ptr_Count)
    {
        ++*Ptr_Count;
    }

    //移动赋值
    void operator=(Share_Ptr && ptr)
    {
        Share_Ptr(std::move(ptr));
    }

    //重载解引用运算符
    T& operator*()
    {
        return *Mem_Ptr;
    }

    T* operator->()
    {
        return Mem_Ptr;
    }







    T* get() const
    {
        return this->Mem_Ptr; 
    }

    size_t use_count() const
    {
        return this->Ptr_Count;
    }
    

};













int main(int argc , char **argv)
{
    
    return 0;
}
