#ifndef __COMPLEX__
#define __COMPLEX__

#include<iostream>
class complex;

//加法
complex&
__doapl(complex* ths , const complex& r);
//减法
complex&
__doami(complex* ths , const complex& r);
//乘法
complex&
__doaml(complex* ths , const complex& r);

//复数的类
class complex
{
public:
    //构造函数
    complex (double r = 0, double i = 0):re (r) , im (i) {}
    //操作符重载
    complex& operator += (const complex&);
    complex& operator -= (const complex&);
    complex& operator *= (const complex&);

    //获取数据(内联函数)
    double real() const { return re; }
    double imag() const { return im; }
private:
    //实部和虚部
    double re , im;

    //友元函数
    friend complex& __doapl(complex* , const complex&);
    friend complex& __doami(complex* , const complex&);
    friend complex& __doaml(complex* , const complex&);
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

inline complex&
__doami(complex* ths , const complex& r)
{
    ths->re -= r.re;
    ths->im -= r.im;
    return *ths;
}

inline complex&
complex::operator -= (const complex& r)
{
    return __doami(this,r);
}

inline complex&
__doaml(complex* ths , const complex& r)
{
    double f = ths->re * r.re - ths->im *r.im;
    ths->im = ths->re * r.im + ths->im * r.re;
    ths->re = f;
    return *ths;
}

inline complex&
complex::operator *= (const complex& r)
{
    return __doaml(this,r);
}

inline double
imag(const complex& x)
{
    return  x.imag();
}

inline double
real(const complex& x)
{
    return x.real();
}

inline complex
operator + (const complex& x , const complex& y)
{
    //complex（）是在创建临时变量
    return complex(real(x) + real(y),
                   imag(x) + imag(y));
}

inline complex
operator + (const complex& x , double y)
{
    return complex(real(x) + y , imag(x));
}

inline complex
operator + (double x , const complex& y)
{
    return complex(x+real(y) , imag(y));
}

inline complex
operator - (const complex& x , const complex& y)
{
    //complex（）是在创建临时变量
    return complex(real(x) - real(y),
                   imag(x) - imag(y));
}

inline complex
operator - (const complex& x , double y)
{
    return complex(real(x) - y , imag(x));
}

inline complex
operator - (double x , const complex& y)
{
    return complex(x - real(y) , imag(y));
}

//cout << c1;  cout << c1 <<endl;  
inline std::ostream&
operator << (std::ostream& os , const complex& x)
{
    return os << '(' << real(x) << ',' << imag(x) << ')';
}
#endif //__MYCOMPLEX__
