#include <stdio.h>
void chline(char ch,int i,int j);
int main()
{
    int i,j;
    char ch;
    scanf("%c",&ch);
    scanf("%d %d",&i,&j);
    chline(ch,i,j);
       return 0;
}
void chline(char ch,int i,int j)
{
    int i_,j_;
    for(i_=0;i_<i;i_++)
    {
        for(j_=0;j_<j;j_++)
            printf("%c",ch);
        printf("\n");
    }
}
