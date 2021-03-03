template <class T> 
struct less : binary_function <T,T,bool> 
{
    bool operator() (const T& x, const T& y) const {return x<y;}
};
