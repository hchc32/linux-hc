#include <iostream>

/*
#define Foo(x) do\
{\
    printf("first statment!\n");\
    printf("second statment!\n");\
}while(0)
*/
#define Foo(x) \
({\
    printf("first statment!\n");\
    printf("second statment!\n");\
})

int main()
{
    
    if(true)
        Foo(x);       
    else
    {}
    return 0;
}
