#include<iostream>
#include<set>

using namespace std;


template<class Key,
         class Compare = less<Key>,
         class Alloc = alloc>
class set
{
public:
    typedef Key Key_type;
    typedef Key value_type;
    typedef Compare Key_compare;
    typedef Compare value_compare;
private:
    typedef rb_tree<Key_type,value_type,_Identity<value_type>,Key_compare,Alloc> rep_type;
    rep_type t;
public:
    typedef typename rep_type::const_iterator iterator;
....
};
