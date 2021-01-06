#include <iostream>
#include<string.h>
#include<cstdlib>
#include<unistd.h>
#include"stack.hpp"
#define NAME_SIZE 20
#define SEXSIZE   5
#define LIVE      1
#define BLOODSIZE 2

//成员信息结点类型
typedef struct Data
{
    char Name_Data[NAME_SIZE];       //成员姓名
    char Sex[SEXSIZE];               //成员性别
    char Father[NAME_SIZE];          //成员父亲
    int  Live_Flag;                  //存活信息
    int  Child_Number;               //孩子数目
    int Married;                    //是否婚配
    struct PedigreeNode *Marry;      //对象
    char blood_type[BLOODSIZE];      //血型    
    int BeiFen;                      //辈份
}DataType;

//族谱结点类型
typedef struct PedigreeNode 
{
    DataType data;                    //结点信息
    struct PedigreeNode *FirstChild;  //第一个孩子指针
    struct PedigreeNode *NextSibling; //右兄弟指针
}PNode,*PTree;

typedef struct pedhead
{
    int Child_Number;                 //孩子数目
    struct PedigreeNode *head;
}Ped_Node,*Ped_Head;

const char *Load = "/home/hc/桌面/123/xinxi.txt";
Ped_Head PHead;
FILE *fp;

//创建族谱
void CreatPTree(Ped_Head *PHead);
//填充结点
void Fill_Data(PNode **Member_Node);
//寻找成员结点
PNode* Find_Member(PNode *Member_Node);
//添加成员
bool Add_Member(PNode *Member_Node);

void CreatPTree(Ped_Head *PHead)
{
    (*PHead) = (Ped_Head)malloc(sizeof(Ped_Node));
    if((*PHead) == NULL)
    {
        printf("malloc error ! line : %d\n",__LINE__);
        exit(0);
    }
}

void Fill_Data(PNode **Member_Node)
{
    (*Member_Node) = (PNode*)malloc(sizeof(PNode));
    if(!feof(fp))
    {
        fscanf(fp,"%s %s %s %d %s %d",(*Member_Node)->data.Name_Data\
                                      ,(*Member_Node)->data.Sex,\
                                      (*Member_Node)->data.Father,\
                                      &(*Member_Node)->data.BeiFen,\
                                      (*Member_Node)->data.blood_type,\
                                      &(*Member_Node)->data.Married);
    }
    //printf("文件在%s处读取完毕",Load);
    /*
    printf("请输入成员姓名:");
    scanf("%s",(*Member_Node)->data.Name_Data);
    printf("请输入性别:");
    scanf("%s",(*Member_Node)->data.Sex);
    printf("请输入父亲姓名:");
    scanf("%s",(*Member_Node)->data.Father);
    printf("请输入你的辈份(第几代):");
    scanf("%d",&(*Member_Node)->data.BeiFen);
    printf("请输入你的血型(A,B,AB,O):");
    scanf("%s",(*Member_Node)->data.blood_type);
    printf("请确认是否婚配?(1/0)\n");
    scanf("%d",&(*Member_Node)->data.Married);
    */
    if((*Member_Node)->data.Married == 0)
    {
        (*Member_Node)->data.Marry = NULL;
    }
    else
    {
        printf("请输入对象的信息:");
        (*Member_Node)->data.Marry = (struct PedigreeNode*)malloc(sizeof(PNode));
        
    }

    /*
    printf("请输入母亲姓名:");
    scanf("%s",(*Member_Node)->data.Mother);
    */

    //getchar();
    (*Member_Node)->data.Live_Flag    = LIVE;
    (*Member_Node)->data.Child_Number = 0;
    (*Member_Node)->FirstChild        = NULL;
    (*Member_Node)->NextSibling       = NULL;
}

PNode* Find_Member(char *Member_Name)
{
    if(PHead == NULL || PHead->head == NULL)
    {
        return NULL;
    }
    stack<PNode*> PStack;
    PStack.push(PHead->head);
    /*
    if(strcmp(PHead->head->data.Name_Data , Member_Name) == 0)
    {
        return PHead->head;
    }
    */
    int number = 0;
    int find_flag = 0;
    PNode *toptemp = NULL;
    while(!PStack.empty() && find_flag == 0)
    {
        toptemp = PStack.top();
        PStack.pop();
        while(toptemp != NULL)
        {
            if(strcmp(toptemp->data.Name_Data , Member_Name) == 0)
            {
                find_flag = 1;
                break;
            }
            
            if(toptemp->FirstChild != NULL)
            {
                PStack.push(toptemp->FirstChild);
            }
            toptemp = toptemp->NextSibling;
        }
    }
    //清空栈
    while(!PStack.empty())
    {
        PStack.pop();
    }
    if(toptemp != NULL)
    {
        return toptemp;
    }
    return NULL;

}


//向族谱添加成员
bool Add_Member(PNode *Member_Node)
{
    Fill_Data(&Member_Node);
    if(PHead->head == NULL)
    {
        PHead->head = Member_Node;
        return true;
    }
    PNode *temp = Find_Member(Member_Node->data.Father);
    //printf("find %s\n",temp->data.Name_Data);

    if(temp->FirstChild == NULL)
    {
        temp->FirstChild = Member_Node;
        return true;
    }
    temp = temp->FirstChild;
    while(temp->NextSibling != NULL)
    {
        temp = temp->NextSibling;
    }
    
    temp->NextSibling = Member_Node;
    return true;
}

void Print_Ped()
{
    printf("Start Print ........\n");
    stack<PNode*> PStack;
    if(PHead == NULL || PHead->head == NULL)
    {
        printf("无\n");
        return ;
    }
    PStack.push(PHead->head);
    int number  = 0;
    int t_count = 0;
    while(!PStack.empty())
    {

        PNode *temp = PStack.top();
        PStack.pop();
        int temp_count = t_count;
        printf("第%d代:\n",t_count);
        printf("\t");
        while(temp != NULL)
        {
            printf("%s ",temp->data.Name_Data);
            if(temp->FirstChild != NULL)
            {
                PStack.push(temp->FirstChild);
            }
            if(temp->NextSibling == NULL)
            {
                break;
            }
            temp = temp->NextSibling;
        }
        printf("\n");
        t_count++;
    }
}

/*
void StandPrint()
{
    if(PHead == NULL || PHead->head == NULL)
    {
        printf("无\n");
        return ;
    }
    stack<PNode*> PStack;
    printf("%s\n",PHead->head->data.Name_Data);
    PStack.push(PHead->head);
    PNode *temp = PStack.top();
    PStack.pop();
    while(temp->FirstChild != NULL)
    {
        PStack.push(temp->FirstChild);
        printf("\t");
        printf("%s\n",temp->FirstChild->data.Name_Data);
        temp = temp->FirstChild;
    }

    while()
    {
        temp = PStack.top();
        PStack.pop();
        temp = temp->NextSibling;
        while(temp->FirstChild != NULL)
        {
            if(temp->FirstChild != NULL)
            {
                
            }
            printf("%s\n",temp->NextSibling->data.Name_Data);
            temp = temp->NextSibling;
        }
        //printf("%s",temp->data.Name_Data);
                
                //A
                //  B
                //      E
                //      F
                //  C
                //  D
                //      G
                //          J
                //      H
                //      I
                //

    }

}
*/

void INore(PTree root1)
{
    PTree root = root1;
    if(root != NULL)
    {
        int number = root->data.BeiFen;
        number--;
        while(number--)
        {
            printf("\t");
        }
        printf("%s\n",root->data.Name_Data);
               //,root->data.Name_Data);
        INore(root->FirstChild);
        INore(root->NextSibling);
    }
}

void DetailINore(PTree root1)
{
    PTree root = root1;
    if(root != NULL)
    {
        int number = root->data.BeiFen;
        number--;
        while(number--)
        {
            printf("\t");
        }
        printf("%s的信息:父亲为%s,辈份为%d,血型为%s\n",root->data.Name_Data\
                                                    ,root->data.Name_Data,\
                                                    root->data.BeiFen,\
                                                    root->data.blood_type);
        DetailINore(root->FirstChild);
        DetailINore(root->NextSibling);
    }
}


int writetofile(const char *file,int number)
{
    PNode *head;
	FILE *fp;
	fp=fopen(file,"wr+");
	if(fp==NULL)
	{
		printf("文件打开失败！");
		return 0;
	}
	while(number--)
	{
        Fill_Data(&head);
		fprintf(fp,"%s %s %s %d %s %d\n",head->data.Name_Data,head->data.Sex,\
                                    head->data.Father,head->data.BeiFen,\
                                    head->data.blood_type,head->data.Married);
	}
	fclose(fp);
    return 1;
}

int main()
{
    CreatPTree(&PHead);
    //writetofile(Load,10);
    fp = fopen(Load,"r");
    if(fp==NULL)
    {
        printf("文件打开失败!");
        return 0;
    }
    for(int i = 0; i < 13 ; i++)
    {
        PNode *Add_Temp = NULL;
        Add_Member(Add_Temp);
    }
    int select;
    while(1)
    {

        printf("*****请输入对应选项******\n");
        printf("*****按照代数打印0*******\n");
        printf("*****按照族谱图打印1*****\n");
        printf("*****详细信息打印2*******\n");
        printf("*****退出系统-1**********\n");
        scanf("%d",&select);
        switch(select)
        {
            case 0:
                {
                    printf("按照代数打印:\n");
                    Print_Ped();
                    break;
                }
            case 1:
                {
                    printf("按照族谱图打印:\n");
                    INore(PHead->head);
                    break;
                }
            case 2:
                {
                    printf("详细信息打印:\n");
                    DetailINore(PHead->head);
                    break;
                }
            case -1:
                {
                    printf("系统即将退出!\n");
                    sleep(1);
                    getchar();
                    exit(0);
                }
        }
    }
    fclose(fp);
    Print_Ped();
    INore(PHead->head);
    return 0;
}
