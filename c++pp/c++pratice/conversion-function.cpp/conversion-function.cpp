#include <iostream>
using namespace std;

class Fraction
{
public:
	//构造函数
	explicit Fraction(int num , int den = 1)
	: m_numerator(num),m_denominator(den){}

	operator double() const
	{ return (double) (m_numerator/m_denominator);}

//	Fraction operator+(const Fraction& f)
//	{ return Fraction(......)};

private:
	int m_numerator;
	int m_denominator;
};

int main()
{
    Fraction f(3,5);
    //类中需要写一个转换函数，将ｆ转为double ,然后在与4相加
    double d=4+f;
    
   //Fraction d2 = f+4;
    return 0;
}
