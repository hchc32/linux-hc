#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include "member.h"
#include "user.h"

int main()
{
    system(" ");
    menu(1);
    return 0;
}

void menu(int flag)
{

	char t[15];
    int select=1,flagpass;
    Node *head = NULL;
    printf("请输入回车键进入系统!\n");
    getchar();
    do
    {
		flagpass = 0;                                         
        printf("\t\t\t\t\t*************************************\n");
        printf("\t\t\t\t\t*        －－－－－－－－－         *\n");
        printf("\t\t\t\t\t*       | 新冠疫情管理系统|         *\n");
        printf("\t\t\t\t\t*        －－－－－－－－－         *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         1.管理员信息注册          *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         2.管理员身份登录          *\n");
        printf("\t\t\t\t\t*                                   *\n");
		printf("\t\t\t\t\t*         3.退出系统                *\n");
		printf("\t\t\t\t\t*                                   *\n");
		printf("\t\t\t\t\t*************************************\n");
    	int select_password;
    	printf("\n\t\t\t\t\t请输入选项：");
    	scanf("%d",&select_password);
		switch(select_password)
		{
			case 1:
			{
				user_register();
				continue;
			}
			case 2:
			{
				flagpass = user_pass();
				
				break;
			}
			case 3:
			{
				exit(1);
			}
			default: 
			{
				printf("\n\t\t\t\t\t请输入正确的选项:\n");
				continue;
			}
		} 
	}
	while(flagpass == 0);
	while(1)
	{
	 	printf("\t\t\t\t\t*************************************\n");
        printf("\t\t\t\t\t*        －－－－－－－－－         *\n");
        printf("\t\t\t\t\t*       | 新冠疫情管理系统|         *\n");
        printf("\t\t\t\t\t*        －－－－－－－－－         *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         1.成员信息录入            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         2.成员信息存储            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         3.成员信息读入            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         4.成员信息显示            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         5.成员信息删除            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         6.成员信息插入            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         7.成员信息修改            *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         8.数据查询                *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         9.数据排序                *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         10.数据统计               *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*         0.退出系统                *\n");
        printf("\t\t\t\t\t*                                   *\n");
        printf("\t\t\t\t\t*************************************\n");
        printf("\n\t\t\t\t\t请输入选项：");
		scanf("%d",&select);
        switch(select)
        {
            case 1://成员信息录入
                {
                	head=Date_write();
                	system("reset");; 
                	printf("\t\t\t\t\t请按任意键继续！\n");
                   	getchar();
                   	getchar();
                	break;
                }
            case 2://成员信息存储
                {
                	int flag;
                	flag = writetofile(f,head); 
                	if( flag == 0 )
                		printf("\t\t\t\t\t无数据可写入！\n");
                	if( flag == 1)
                		printf("\n\t\t\t\t\t数据写入文件成功！存储在%s处\n",f);
                	printf("\t\t\t\t\t请按回车键继续！\n");
                	getchar();
                	getchar();
					system("reset");;
                	
                	break;
                }
            case 3://成员信息读入
				{
					head = readfromfile(f,head);
					if( head == NULL )
					{
						break;
						system("reset");;
					}
					else
					{
						getchar();
						getchar();
						system("reset");;
					}
					break;
				}	 
            case 4://成员信息显示
                   {
                   	Data_print(head);
                   	printf("\t\t\t\t\t请按回车键继续！\n");
                   	getchar();
                   	getchar();
                   	system("reset");;
                   	break;
                   }
            case 5://成员信息删除
                   {
                   		Data_delete(head);
                   	 	break;
                   }
            case 6://成员信息插入
                   {
                   		Data_insert(head);
                   	 	break;
                   }
            case 7://成员信息修改
                   {
                   		 Data_revise(head);
                   		 getchar(); 
                   		 break;
                   }
            case 8://数据查询
                   {
                   		int select_find;
                   		system("reset");; 
                   		printf("\t\t\t\t\t************数据查询界面***************\n");
                   		printf("\t\t\t\t\t*                                     *\n"); 
						printf("\t\t\t\t\t*           1.按成员编号              *\n");
						printf("\t\t\t\t\t*                                     *\n");
						printf("\t\t\t\t\t*           2.按照成员姓名            *\n");
						printf("\t\t\t\t\t*                                     *\n");
						printf("\t\t\t\t\t*       3.按照目前状态和所在省份      *\n");
						printf("\t\t\t\t\t*                                     *\n");
						printf("\t\t\t\t\t***************************************\n");
						printf("\n\t\t\t\t\t请输入选项：");
						scanf("%d",&select_find);
						switch(select_find)
						{
							case 1:
							{
								Data_Find_membernum(head);
								printf("\t\t\t\t\t请按回车键回到主目录！\n");
								getchar();
								getchar();
								system("reset");;
								break;
							}
							case 2:
							{
								Data_Find_name(head);
								printf("\t\t\t\t\t请按回车键回到主目录！\n");
								getchar();
								getchar();
								system("reset");; 
								break;		
							}
							case 3:
							{
								Data_Find_state_province(head);
								printf("\t\t\t\t\t请按回车键回到主目录！\n");
								getchar();
								getchar();
								system("reset");;
								break;
							}
						}		 
                   	 	break;
                   }
            case 9://数据排序
                   {
	                   	 if(head == NULL)
	    				{
					    	system("reset");; 
					    	printf("\n\t\t\t\t\t没有可以排序的数据!\n");
					    	printf("\n\t\t\t\t\t请按回车键返回目录\n");
					    	getchar();
					    	getchar();
					    	break;
						}
                   		printf("\n\t\t\t\t\t请问你要按何种顺序排列? 1/升序,0/降序\n");
                   		int order;
                   		scanf("%d",&order);
	                   	head = Data_Sort(head,order);
                   		break;
                   }
            case 10://数据统计
                   {
                   		Data_gather_state(head);
                   		break;
                   }
            case 0://系统关闭
				  {
			 		exit(1);
         		  }
    	}
	}
}

LinkList	Date_write(void)
{
	Node *head = NULL,*p1 = NULL,*p2 = NULL;
	int count,i;
	printf("\t\t\t\t\t请输入你要录入信息的成员个数：\n");
	scanf("%d",&count);
	head=p1 = (Node*)malloc(sizeof(Node));
	printf("	注：目前状态填写格式（请务必如实填写） : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	for(  i = 0; i < count ; i++ )
	{
		p2 = (Node*)malloc(sizeof(Node));
		while(1)
		{
			scanf("%d %s %s %s %s %s %s %s %d",&p2->member_num,p2->name,\
			p2->sex,p2->ID_num,p2->phone_num,p2->province,p2->city,p2->address,&p2->present_state);
			if(strcmp(p2->sex ,"男" ) == 0|| strcmp(p2->sex ,"女") == 0)
			{
				if(p2->present_state == 0 || p2->present_state ==1 || p2->present_state == 2 || p2->present_state == 3 )
					break;
				else
				{
				printf("\t\t\t\t\t请输入有效数据\n"); 
				continue;	
				} 
			}
			else
			{
				printf("\t\t\t\t\t请输入有效数据\n"); 
				continue;	
			} 
		}
		
		p1->next = p2;
		p1 = p2;
	}
	p2->next = NULL;
	return head;
}

void	Data_print(Node *head)
{
	if(head == NULL)
	{
		printf("\t\t\t\t\t无信息可显示！\n");
		return ;
	}
	head = head->next;
	printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	printf("	|成员编号||—姓名 ||—性别 ||—证件号码 —||—手机号码 —||—省份 —||—市 —||—详细地址—||—目前状态—|\n"); 
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	while(head != NULL)
	{
		printf(" 	%4d       %8s    %s   %-19s %s   %-4s   %-4s   %-12s  %10d\n",head->member_num,head->name,\
		head->sex,head->ID_num,head->phone_num,head->province,head->city,head->address,head->present_state);
		head = head->next;
	}
}

int writetofile(char *f,Node *head)
{
    Node *p;
	FILE *fp;
	fp = fopen(f,"w+");
	if(fp == NULL)
	{
		printf("\t\t\t\t\t文件无法打开！\n");
		return 0;
	}
	if(head == NULL)
	{
		return 0;
	}
	p = head->next;
	while(p != NULL)
	{
		fprintf(fp,"%d %s %s %s %s %s %s %s %d\n",p->member_num,p->name,\
		p->sex,p->ID_num,p->phone_num,p->province,p->city,p->address,p->present_state);
        p = p->next;
	}
	fclose(fp);
    return 1;
}

LinkList 	readfromfile(char *f,Node *head)
{
    int i,flag = 0;
    FILE *fp;
    Node *p1=NULL,*p2=NULL;
    fp = fopen(f,"r");
    if(fp == NULL)
    {
        printf("\t\t\t\t\t文件打开失败!\n");
        return NULL;
    }
    fseek(fp, 0L,SEEK_END);
	int len =ftell(fp);            //防止文件没有数据而导致读取不确定的位置 
	if(len == 0)
	{
		printf("文件没有数据!\n");
		return NULL;
	}
	rewind(fp); 
    head = p1 = (Node *)malloc(sizeof(Node));
	p2 = (Node *)malloc(sizeof(Node)); 
    while(!feof(fp) )              //增加一个文件大小，防止乱码 
    {
    	flag = 1;
       	fscanf(fp,"%d %s %s %s %s %s %s %s %d\n",&p2->member_num,p2->name,\
		p2->sex,p2->ID_num,p2->phone_num,p2->province,p2->city,p2->address,&p2->present_state);
        p1->next = p2;
        p1 = p2;
		p2 = (Node*)malloc(sizeof(Node));
    }
    if(flag == 0)
    	head=NULL;
    free(p2);
    p1->next = NULL;
    fclose(fp);
    printf("\t\t\t\t\t位于%s处的文件数据读入成功！\n",f);
    return head;
}

int		Data_Find_membernum(Node *head)
{
		int membernum,flag;
		if(head == NULL)
		{
			printf("\t\t\t\t\t数据为空，无法查询！\n");
			return 0;
		}
		printf("\t\t\t\t\t请输入你要查询的成员编号:\n");
		scanf("%d",&membernum);
		head = head->next;
		printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	    printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
		printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	    printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
		while(head != NULL)
		{
			if(head -> member_num == membernum)
			{
				flag++;
				printf("	%4d       %8s    %s    %s %s   %-4s   %-4s   %-10s   %10d\n",head->member_num,head->name,\
						head->sex,head->ID_num,head->phone_num,head->province,head->city,head->address,head->present_state);
			}
			head = head->next;
		}
		if( !flag )
			printf("\n\t\t\t\t\t数据未找到！\n");
		else
		{
			printf("\n\t\t\t\t\t数据查找完毕!\n");
		}
		return 0;
		
} 


int		Data_Find_name(Node *head)
{
		char name_find[20]; 
		int flag;
		if(head == NULL)
		{
			printf("\t\t\t\t\t数据为空，无法查询！\n");
			return 0;
		}
		printf("\t\t\t\t\t请输入你要查找的成员姓名:\n");
		scanf("%s",name_find);
		head = head->next;
		printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	    printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
		printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	    printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
		while(head != NULL)
		{
			if(strcmp(name_find,head->name) == 0)
			{
				flag++;
				printf("	%4d       %8s    %s    %s %s   %-4s   %-4s   %-10s   %10d\n",head->member_num,head->name,\
						head->sex,head->ID_num,head->phone_num,head->province,head->city,head->address,head->present_state);
			}
			head = head->next;
		}
		if( !flag )
		{
			system("reset");;
				printf("\n\t\t\t\t\t数据未找到！\n");
		}
		else
		{
			printf("\n\t\t\t\t\t数据查找完毕!\n");
		}
		return 0;
} 

int		Data_Find_state_province(Node *head)
{
		char province_find[20];
		int state_find,flag = 0;
		if(head == NULL)
		{
			printf("\t\t\t\t\t数据为空，无法查询！\n");
			return 0;
		}
		printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
		printf("\t\t\t\t\t请输入你要查询成员的所在省份，以及目前状况:\n");
		scanf("%s%d",province_find,&state_find); 
		head = head->next;
		printf("\n\t\t\t\t\t你要查找的数据如下:\n");
		printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	    printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
		printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	    printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
		while(head != NULL)
		{
			if(strcmp(province_find,head->province) == 0 && head->present_state == state_find )
			{
				flag = 1;
				printf("	%4d       %8s    %s    %s %s   %-4s   %-4s   %-10s   %10d\n",head->member_num,head->name,\
						head->sex,head->ID_num,head->phone_num,head->province,head->city,head->address,head->present_state);
			}
			head = head->next;
		}
		if( !flag )
		{
			system("reset");;
			printf("\n\t\t\t\t\t数据未找到！\n");
		}
		else
			printf("\t\t\t\t\t数据查询打印完毕！\n");
		return 0;
} 

int   Data_revise(Node *head)
{
	char revise_name[20];
	Node *temp = head;
	int select_revise,flag = 0,num_revise;
	if(head == NULL)
	{
		printf("\t\t\t\t\t数据为空，无法修改！\n");
		return 0;
	}
	head = head->next;
	printf("\t\t\t\t\t请输入你要修改成员信息的姓名: ");
	scanf("%s",revise_name);
	printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	while( head != NULL )
	{
		if(strcmp(revise_name,head->name) == 0)
		{
			flag++;
			printf("	%4d       %8s    %s    %s %s   %-4s   %-4s   %-10s   %10d\n",head->member_num,head->name,\
					head->sex,head->ID_num,head->phone_num,head->province,head->city,head->address,head->present_state);
		break; 
		}
		head = head->next;
	}
		if(flag == 1)
		{
			printf("	%s信息如上，请问你要修改哪项信息？(1 成员编号 2 姓名.......依次类推，若不修改请输入0)\n",revise_name);
			scanf("%d",&select_revise);
		}
		else if( flag > 1 )
		{
			printf("\n\t\t\t\t\t有多个成员，请输入对应的成员编号进行修改：");
			scanf("%d",&num_revise);
			while( temp != NULL )
			{
				if(num_revise == temp->member_num )
				{
					break;
				}
				temp = temp->next;
			}
			head = temp;
			printf("%s信息如上，请问你要修改哪项信息？(1 成员编号 2 姓名.......依次类推，若不修改请输入0)\n",revise_name);
			scanf("%d",&select_revise);
		}
			switch(select_revise)
			{
				case 1:
				{
					int num_revise;
					printf("\t\t\t\t\t请输入新的成员编号：\n");
					scanf("%d",&num_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						head->member_num = num_revise;	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
				break;
				}
				case 2:
				{
					char name_revise[20];
					printf("\t\t\t\t\t请输入新的成员姓名：\n");
					scanf("%s",name_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->name,name_revise);	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
					break;
				}
				case 3:
				{
					char sex_revise[4];
					printf("\t\t\t\t\t请输入新的成员性别：\n");
					scanf("%s",sex_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->sex , sex_revise);	
						printf("\t\t\t\t\t修改成功！\n");
						getchar();	
						return 1;
					}
					break;
				}
				case 4:
				{
					char ID_revise[20];
					printf("\t\t\t\t\t请输入新的成员证件号码：\n");
					scanf("%s",ID_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->ID_num , ID_revise);	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
					break;
				}	
				case 5:
				{
					char phone_revise[15];
					printf("\t\t\t\t\t请输入新的成员手机号码：\n");
					scanf("%s",phone_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->phone_num , phone_revise);	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
					break;
				}	
				case 6:
				{
					char province_revise[10];
					printf("\t\t\t\t\t请输入新的成员所在省份：\n");
					scanf("%s",province_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->province , province_revise);	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
					break;
				}	
				case 7:
				{
					char city_revise[20];
					printf("\t\t\t\t\t请输入新的成员所在城市：\n");
					scanf("%s",city_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->city , city_revise);	
						printf("\t\t\t\t\t修改成功！\n");
						getchar();	
						return 1;
					}
					break;
				}	
				case 8:
				{
					char address_revise[30];
					printf("\t\t\t\t\t请输入新的成员详细地址：\n");
					scanf("%s",address_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						strcpy(head->address , address_revise);	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
					break;
				}	
				case 9:
				{
					int state_revise;
					printf("\t\t\t\t\t请输入新的成员目前状态:\n");
					scanf("%d",&state_revise);
					printf("\t\t\t\t\t确认修改？（1/Y，0/N）\n");
					int flag;
					scanf("%d",&flag);
					if(flag == 1)
					{
						head->present_state = state_revise;	
						printf("\t\t\t\t\t修改成功！\n");	
						getchar();
						return 1;
					}
					break;
				}
				case 0:
				{
					return 1;
				}	
			} 
	if( !flag )
	{
		printf("\t\t\t\t\t无该成员信息，无法修改!\n");
		getchar();
		getchar();
		system("reset");;
		printf("\t\t\t\t\t请按回车键回到主目录！\n");
	}
}

void   Data_delete(Node *head)
{
	Node *p,*temp = head;
	char delete_name[20];
	int flag = 0,count = 0,sure;
	if(head == NULL)
	{
		printf("\t\t\t\t\t数据为空，无法删除！\n");
		return;
	}
	p = head;
	head = head->next;
	printf("\t\t\t\t\t请输入你要删除成员信息的姓名: ");
	scanf("%s",delete_name);
	printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	while( head != NULL )
	{
		if(strcmp(delete_name,head->name) == 0)
		{
			int sure;
			flag = 1;
			printf("	%4d       %8s    %s    %s %s   %-4s   %-4s   %-10s   %10d\n",head->member_num,head->name,\
					head->sex,head->ID_num,head->phone_num,head->province,head->city,head->address,head->present_state);
			count++;
			break;
		}
		p = head;
		head = head->next;
	}
	if( !count )
	{
		printf("\t\t\t\t\t无该成员信息，无法删除!\n");
		getchar();
		getchar();
		system("reset");;
		printf("\t\t\t\t\t请按回车键回到主目录！\n");
	}
	else if(count == 1)
	{
		printf("\n\t\t\t\t\t%s的信息如上，你确定要删除吗？(1/Y,0/2)\n",delete_name); 
		scanf("%d",&sure);
		if( !sure )
		{
			printf("\n\t\t\t\t\t删除已取消！\n");
			return;
		}
		else
		{
			p->next = head->next;
			free(head);
			printf("\t\t\t\t\t%s的信息删除成功！\n",delete_name);
			getchar();
			getchar();
			system("reset");;
			printf("\t\t\t\t\t请按回车键回到主目录！\n");
			return;
		}
	}
	else if(count > 1)
	{
		printf("\n\t\t\t\t\t请问你要删除哪个成员编号对应的内容？\n");
		int num_dele;
		scanf("%d",&num_dele);
		 while( temp != NULL )
		{
			if( num_dele == temp->member_num  )
			{
				printf("\n\t\t\t\t\t%s的信息如上，你确定要删除吗？(1/Y,0/2)\n",delete_name); 
				scanf("%d",&sure);
				if( !sure )
				{
					printf("\n\t\t\t\t\t删除已取消！\n");
					return; 
				}
				else
				{
					p->next = temp->next;
					free(temp);
					printf("\t\t\t\t\t%s的信息删除成功！\n",delete_name);
					getchar();
					getchar();
					system("reset");;
					printf("\t\t\t\t\t请按回车键回到主目录！\n");
					return ;
				}
			}
			p = temp;
			temp = temp->next;
		}
	}
}

void 	Data_insert(Node *head)
{
	if(head == NULL)
	{
		system("reset");; 
		printf("\n\t\t\t\t\t数据为空，无法插入！\n");
		printf("\n\t\t\t\t\t按回车键返回主目录\n");
		getchar();
		getchar();
		return ;
	}
	Node *insert_data,*p;
	insert_data = (Node *)malloc(sizeof(Node));
	printf("\n\t\t\t\t\t请输入你要插入的数据:\n");
	printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	printf("	|成员编号||—姓名—||—性别—||—证件号码—||—手机号码—||—省份—||—市—||—详细地址—||—目前状态|\n"); 
	printf(" 	－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－\n"); 
	scanf("%d %s %s %s %s %s %s %s %d",&insert_data->member_num,insert_data->name,\
	insert_data->sex,insert_data->ID_num,insert_data->phone_num,insert_data->province,insert_data->city,insert_data->address,&insert_data->present_state);
	int n;
	printf("\n\t\t\t\t\t请问你要插入到第几行？（请输入一个正整数）\n");
	scanf("%d",&n);
	p = head;
	int i;
	for( i = 0 ; i < n ; i++ )
	{
		p = head;
		head = head->next;
		if(head == NULL)
		{
			printf("\n\t\t\t\t\t插入失败！数据没有这么多行！\n");
			printf("\n\t\t\t\t\t请问你是要往最后一行插入吗？(1/Y,0/N)\n");
			int selete;
			scanf("%d",&selete);
			if( !selete )
			{
				printf("\n\t\t\t\t\t取消操作成功！\n");
				getchar();
				getchar();
				return ;
			}
			else
			{
				p->next = insert_data;
				insert_data->next = NULL;
				getchar();getchar();
				printf("\n\t\t\t\t\t末尾插入成功!\n");
				return ; 
			}
		} 
	}
	p -> next = insert_data;
	insert_data->next = head;
	system("reset");;
	printf("\n\t\t\t\t\t插入操作成功!\n");		
	getchar();
	getchar();
} 
//数据汇总 
int 	Data_gather_state(Node *head)
{
	if(head == NULL)
	{
		system("reset");;
		printf("\n\t\t\t\t\t无数据！\n");
		printf("\n\t\t\t\t\t按回车键返回主目录\n");
		getchar();
		getchar();
		return 0;
	} 
	printf("	注：目前状态含义为 : 0 状态正常 1 确诊病例 2 疑似病例 3 近期接触过湖北籍的人\n");
	printf("\n\t\t\t\t\t请输入你要统计哪个状态的人数: ");
	int gather_state,gather_count = 0;
	scanf("%d",&gather_state);
	head = head->next;
	while( head != NULL )
	{
			if( head->present_state == gather_state )
			{
				gather_count++;
			}
			head = head->next;
	}
	if( gather_state == 0 )
	{
		printf("\n\t\t\t\t\t状态正常的人数为:%d人\n",gather_count);
		getchar();
		getchar();
	}
	if( gather_state == 1 )
	{
		printf("\n\t\t\t\t\t确诊病例的人数为:%d人\n",gather_count);
		getchar();
		getchar();
	} 
	if( gather_state == 2 )
	{
		printf("\n\t\t\t\t\t疑似病例的人数为:%d人\n",gather_count);
		getchar();
		getchar();
	}  
	if( gather_state == 3)
	{
		printf("\n\t\t\t\t\t近期接触过湖北籍的人数为:%d人\n",gather_count);
		getchar();
		getchar();
	} 
}


//管理员信息注册
void 	user_register(void)
{
	User regis;
	printf("\n\t\t\t\t\t请输入你要注册的用户名：");
	scanf("%s",regis.username);
	printf("\n\t\t\t\t\t请输入你的密码：");
	scanf("%s",regis.password);
	printf("\n\t\t\t\t\t请输入");
	FILE *fp;
	fp = fopen(u,"a+");
	if(fp == NULL)
	{
	 	printf("\n\t\t\t\t\t文件打开失败！\n");
		return ; 
	} 
	fprintf(fp,"%s %s\n",regis.username,regis.password);
	fclose(fp);
	system("reset");; 
	printf("\n\t\t\t\t\t注册成功！\n");
	printf("\n\t\t\t\t\t按回车键返回主目录\n");
	getchar();
	getchar(); 
}

//管理员登录 
int 	user_pass(void)
{
	User pass,temp;
	FILE *fp;
	fp = fopen(u,"r"); 
	if(fp == NULL)
	{
		printf("\n\t\t\t\t\t文件打开失败！\n");
		return 0;
	}
	printf("\n\t\t\t\t\t请输入你登录用户名：");
	scanf("%s",pass.username);
	printf("\n\t\t\t\t\t请输入你的密码：");
	getchar();
	int i = 0;
	char ch; 
	while( 1 )
	{
		pass.password[i] = getch();
		if(pass.password[i] =='\r')
		{
			pass.password[i] = '\0';
			break;
		}
		if(pass.password[i] == 0x7f)
		{
			--i;
			printf("\b \b");        //用空格代替* 
			continue;
		}
		putchar('*');
		++i;
	}

	while( !feof(fp) )
	{
		fscanf(fp,"%s %s",&temp.username ,&temp.password );
		if(strcmp(pass.username,temp.username) == 0 && strcmp(pass.password,temp.password)  == 0 )
		{
			strcat(f,pass.username);
			strcat(f,".txt");
			printf("\n\t\t\t\t\t登录成功！\n");
			printf("\n\t\t\t\t\t按回车键进入！\n");
			getchar();
			system("reset");; 
			fclose(fp);
			return 1; 
		}
	}
	printf("\n\t\t\t\t\t用户名或者密码错误!\n");
	printf("\n\t\t\t\t\t请按回车键返回目录\n");
	fclose(fp);
	getchar();
	system("reset");; 
	return 0;
}

Node * Data_Sort(Node *List,int order)
{
    Node * p, * q, * tail;

    tail = NULL;
    while((List->next->next) != tail)
    {
        p = List;
        q = List->next;
        while(q->next != tail)
        {
        	if(order == 1)
	        {
			    if((q->member_num) > (q->next->member_num))
	            {
	                p->next = q->next;
	                q->next = q->next->next;
	                p->next->next = q;
	                q = p->next;
	            }
	    	}
	    	if(order == 0)
	    	{
			    if((q->member_num) < (q->next->member_num))
	            {
	                p->next = q->next;
	                q->next = q->next->next;
	                p->next->next = q;
	                q = p->next;
	            }
	    	}
	    	
            q = q->next;
            p = p->next;
        }
        tail = q;
    }
    system("reset");;
    printf("\n\t\t\t\t\t数据排序成功！\n");
	printf("\n\t\t\t\t\t请按回车键返回主目录!\n"); 
    getchar();
    getchar();
    return List;
}

int getch(void)
{
     struct termios tm, tm_old;
     int fd = 0, ch;

     if (tcgetattr(fd, &tm) < 0) 
     {
          return -1;
     }

     tm_old = tm;
     cfmakeraw(&tm);//更改终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理
     if (tcsetattr(fd, TCSANOW, &tm) < 0) 
     {
          return -1;
     }

     ch = getchar();
     if (tcsetattr(fd, TCSANOW, &tm_old) < 0) 
     {
          return -1;
     }
     return ch;
}


