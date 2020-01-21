#include<stdio.h>
#define N 5
void copy_arr(double* target, double* source, int n);
void copy_ptr(double* target, double* source, int n);
void copy_ptrs(double* target, double* source, double* end);
void put_array(double* ar, int n);
int main()
{
    double source[N] = {1.1,2.2,3.3,4.4,5.5};
    double target1[N];
    double target2[N];
    double target3[N];
    copy_arr(target1,source,N);
    copy_ptr(target2,source,N);
    copy_ptrs(target3,source,source+N);
    put_array(target1,N);
    put_array(target2,N);
    put_array(target3,N);
    return 0;
}
void copy_arr(double* target, double* source, int n)
{
    int i;
    for (i = 0; i < n; i++)
        target[i] = source[i];
}
void copy_ptr(double* target, double* source, int n)
{
    double* p;
    for (p = source; p < source + n; p++, target++)
        * target = *p;
}
void copy_ptrs(double* target, double* source, double* end)
{
    for (; source < end; source++, target++)
        * target = *source;
}
void put_array(double* ar, int n)
{
    double* p;
    for (p = ar; p < ar + n; p++)
        printf("%.1f ", * p);
}
