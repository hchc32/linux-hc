#include<stdio.h>
void copy_ptrs(int* target, int* source, int* end);
void put_array(int* ar, int n);
int main(void)
{
    int source[7] = { 1,2,3,4,5,6,7 };
    int target[3];
    put_array(source, 7);
    copy_ptrs(target, source + 2, source + 5);
    put_array(target, 3);
    return 0;
}
void copy_ptrs(int* target, int* source, int* end)
{
    for (; source < end; source++, target++)
        * target = *source;
}
void put_array(int* ar, int n)
{
    int* p;
    for (p = ar; p < ar + n; p++)
        printf("%d ", *p);
}
