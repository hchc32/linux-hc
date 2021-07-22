#include <iostream>

template<typename T, typename Sig>                                 
struct has_foo {                     
    template <typename U, U> struct type_check;
    template <typename V> static char (& chk(type_check<Sig, &V::foo>*))[1];
    template <typename  > static char (& chk(...))[2]; 
    static bool const value = (sizeof(chk<T>(0)) == 1);
};

struct A {
    void foo();
};

struct B : A {};

int main()
{
    using namespace std;
    cout << boolalpha << has_foo<A, void (A::*)()>::value << endl; // true
    cout << boolalpha << has_foo<B, void (B::*)()>::value << endl; // false
}
