#ifndef MUDUO_BASE_NONCOPYABLE_H
#define MUDUO_BASE_NONCOPYABLE_H

namespace muduo
{
//不可以复制的类
//子类会通过私有化父类noncopyable的复制构造函数，
//从而禁止用户从外部访问拷贝构造函数和拷贝赋值函数.
class noncopyable
{
public:
    noncopyable(const noncopyable &) = delete;
    void operator=(const noncopyable &) = delete;

//保证构造函数和析构函数只能在子类调用，不能在外部的类中调用
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

} // namespace muduo





#endif
