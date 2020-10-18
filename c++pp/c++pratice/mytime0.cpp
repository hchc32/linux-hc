#include <iostream>
#include"mytime0.h"

using namespace std;

Time::Time()
{
    hours = minutes = 0;
}

Time::Time(int h , int m)
{
    hours = h;
    minutes = m;
}

void Time::AddMin(int m)
{
    minutes += m;
    hours += minutes / 60;
    minutes %= 60;
}



int main()
{
    
    return 0;
}
