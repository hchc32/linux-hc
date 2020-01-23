#include <stdio.h>
#include <stdlib.h>

int main()
{
    float a,b;
    scanf("%f %f", &a, &b);
    printf("At %.2f megabits per second, a file of %.2f megabytes\ndownloads in %.2f seconds.\n", a, b, b/a);
    return 0;
}

