#include<iostream>
template<class Key,
         class T,
         class Compare = less<Key>,
         class Alloc = alloc
        >
class map
{
public:
    typedef Key Key_type;
    typedef T data_type;
    typedef T mapped_type;
    //const Key,key不能用迭代器更改,data可以.
    typedef pair<const Key,T> value_type;
    typedef Compare Key_compare;
private:
    //红黑树
    typedef rb_tree<Key_type,value_type,select1st<value_type>,Key_compare,Alloc> rep_type;
    rep_type t;

public:
    typedef typename rep_type::iterator iterator;
    ...
};

