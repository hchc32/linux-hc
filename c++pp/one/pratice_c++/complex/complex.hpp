#ifndef __COMPLEX__
#define __COMPLEX__


//复数的类
class complex
{
public:
    //构造函数
    complex (double r = 0, double i = 0 )
        :re (r) , im (i)
    {  }
    
    //操作符重载
    complex& operator += (const complex&);

    //获取数据(内联函数)
    double real() const { return re; }
    double imag() const { return im; }
private:
    //实部和虚部
    double re , im;

    //友元函数
    friend complex& __doapl(complex* , const complex&);
};

inline complex& 
__doapl(complex* ths , const complex& r)
{
    ths->re += r.re;
    ths->im += r.im;
    return *ths;
}

//void complex::operator += (const complex& r)
inline complex& 
complex::operator += (const complex& r)
{
    return __doapl(this,r);
}

#endif
