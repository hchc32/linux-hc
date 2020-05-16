#include <stdio.h>
#include<readline/history.h>

void ShowHistory()
{
    int i = 0;
    HIST_ENTRY ** his;
    his = history_list();
    while(his[i] != NULL)
    {
        printf("%s\n", his[i]->line);
        i++;
    }
}
int main()
{
    ShowHistory();
    return 0;
}

