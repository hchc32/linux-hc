#include <termio.h>
//成员信息 
typedef struct member
{
    int member_num;    //成员编号
    char name[20];     //成员姓名 
    char sex[4];       //成员性别
    char ID_num[20];   //证件号码
    char phone_num[15];//手机号码
    char province[10]; //所在省份
    char city[20];     //所在城市
    char address[30];  //详细地址
    int present_state; 
    //目前状态:0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人
	struct member *next; 
}Node,*LinkList;


//成员信息存储绝对路径名 
char f[]="/home/hc/桌面/1.txt";

void	menu(int flag); 	   							//菜单模块函数 
LinkList	Date_write(void);							//信息录入函数 
void	Data_print(Node *head);							//信息显示函数
int writetofile(char *f,Node *head);					//数据存储函数
LinkList	readfromfile(char *f,Node *head);			//数据读入函数 
int		Data_Find_membernum(Node *head);				//数据查询函数(按照成员编号查询) 
int		Data_Find_name(Node *head);					    //数据查询函数(按照成员姓名查询) 
int		Data_Find_state_province(Node *head);           //数据查询函数(按照所在省份和目前状态查询) 
int   Data_revise(Node *head);                          //数据修改函数 
void		Data_delete(Node *head);                    //数据删除函数 
void 	Data_insert(Node *head);                        //数据插入函数 
int 	Data_gather_state(Node *head);					//数据汇总(只汇总相应状态的病人人数) 
Node * Data_Sort(Node *List,int order);                 //数据排序函数         
int getch(void);
