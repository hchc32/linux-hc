#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define LEN 4 
struct month{
    char name[LEN];
    int days;
    int monumb;
};
struct month months[12] = {{"Jan", 31, 1},{"Feb", 28, 2},{"Mar", 31, 3},{"Apr", 30, 4},{"May", 31, 5},
                            {"Jun", 30, 6},{"Jul", 31, 7},{"Aug", 31, 8},{"Sep", 30, 9},{"Oct", 31, 10},
                            {"Nov", 30, 11},{"Dec", 31, 12},
                          };
int main(void)
{
    char monthname[LEN];
    int total = 0;
    int i;
    printf("Enter the name of the month (first three letters) : ");
    fgets(monthname, LEN, stdin);
    for(i = 0; i < LEN; i++)
    {
        if(i==0)
            monthname[i] = toupper(monthname[i]);
        else
            monthname[i] = tolower(monthname[i]);
    }
    for(i=0;i<12;i++)
    {
        total+=months[i].days;  
        if(strcmp(monthname, months[i].name) == 0)
        {
            break;
        }
    }
    printf("The total days of these months before your month are %d days.\n",total);
    return 0;
}
