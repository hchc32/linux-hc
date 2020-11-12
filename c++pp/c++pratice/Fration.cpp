#include <iostream>

class Fraction
{
public:
    Fraction(int num,int den = 1)
    : m_numerator(num),m_denominator(den){}

    operator double() const
    { return (double) (m_numerator / m_denominator); }

    Fraction operator+(const Fraction& f)
    { return Fraction(1,1); }
private:
    int m_numerator;
    int m_denominator;
};

int main()
{
    Fraction f(3,5);
    Fraction d2 = f+4;
    return 0;
}
