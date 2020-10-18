//稀疏矩阵三元组顺序表
#include <stdio.h>
#include<stdlib.h>

#define MAXSIZE 100
typedef int ElementType;

//结点类型
typedef struct
{
    int row , col;
    ElementType value;
}Triple;

//三元组顺序表结构
typedef struct
{
    Triple data[MAXSIZE];
    int rows , cols , nums;
}TSMatrix;

//输入矩阵
void InputMatrix(TSMatrix **tsm)
{
    int inum , jnum , temp;
    (*tsm) = (TSMatrix*)malloc(sizeof(TSMatrix));
    if((*tsm) == NULL)
    {
        printf("malloc error !");
        exit(1);
    }
    (*tsm)->nums = 0;
    scanf("%d,%d",&inum,&jnum);
    (*tsm)->rows = inum;
    (*tsm)->cols = jnum;
    int k = 0;
    for(int i = 0; i < inum ; i++)
    {
        for(int j = 0; j < jnum ; j++)
        {
            scanf("%d",&temp);
            if(temp == 0)
            {
                continue;
            }
            else
            {
                (*tsm)->nums++;
                (*tsm)->data[k].row = i+1;
                (*tsm)->data[k].col = j+1;
                (*tsm)->data[k++].value = temp;
            }
        }
    }
}

//打印三元组
void display(TSMatrix *tsm)
{
    int i = 0 ;
    printf("rows = %d , cols = %d , nums = %d\n\n",tsm->rows,tsm->cols,tsm->nums);
        printf("    row      col      value\n");
    while( i <= tsm->nums )
    {
        printf(" %5d     %5d      %5d\n",tsm->data[i].row,tsm->data[i].col,tsm->data[i].value);
        i++;
    }
}

//矩阵转置1
void TransposeTSMatrix3(TSMatrix A , TSMatrix *B)
{
    B->rows = A.cols;
    B->cols = A.rows;
    B->nums = A.nums;
    int i = 0;
    while( i < B->nums )
    {
        int min = 0;
        for(int j = 1 ; j < B->nums ; j++)
        {
            if(A.data[min].col > A.data[j].col) min =  j;
        }
        B->data[i].row = A.data[min].col;
        B->data[i].col = A.data[min].row;
        B->data[i].value = A.data[min].value;
        i++;
        A.data[min].col = A.cols+1;
    }
}

//一次定位快速转置
void Fast_TransposeTSMatrix(TSMatrix *A , TSMatrix *B)
{
    //存放每一列的非零元素个数
    int num[MAXSIZE];
    //存放转置矩阵每一行在三元表中的开始位置
    int position[MAXSIZE];    
    B->cols = A->rows;
    B->rows = A->cols;
    B->nums = A->nums;
    if(B->nums > 0)
    {
        //初始化num数组
        for(int i = 1; i <= A->cols ; i++)
        {
            num[i] = 0;
        }
        //赋值给ｎｕｍ数组
        for(int i = 0; i < A->nums ; i++)
        {
            num[A->data[i].col]++;
        }
        
        /*
        for(int i = 1; i <= A->cols ;i++)
        {
            printf("%d ",num[i]);
        }
        */
        //赋值给position数组
        position[1] = 1;
        for(int col = 2 ; col <= A->cols ; col++)
        {
            position[col] = position[col-1] + num[col-1];
        }
        /*
        for(int col = 1 ; col <= A->cols ; col++)
        {
            printf("%d ",position[col]);
        }
        */
        for(int p = 0; p < A->nums ; p++)
        {
            //定位操作   
            int col = A->data[p].col;
            int q = position[col];

            B->data[q].col = A->data[p].row;
            B->data[q].row = A->data[p].col;
            B->data[q].value = A->data[p].value;
            
            position[col]++;
        }
        
    }
}

int main()
{
    TSMatrix *tsm;
    TSMatrix B;
    InputMatrix(&tsm);
   
    display(tsm);
   
    //TransposeTSMatrix3((*tsm),&B);
    Fast_TransposeTSMatrix(tsm,&B);

    display(&B);
    return 0;
}

