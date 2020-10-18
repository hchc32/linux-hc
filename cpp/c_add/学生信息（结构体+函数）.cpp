#include<stdio.h>
#include<conio.h>
struct student input_student();    //输入学生信息 
void display_student(struct student stu);    //显示学生信息 

struct student    //定义结构体 
{
	char name[9];
	char sex[3];
	int year,mouth,ri;
	float score;
};
 
main()
{
	struct student stu1,stu2;      				//引用结构体
	printf("请输入第一个学生的信息：\n"); 
	stu1=input_student();               		//函数调用 
	printf("请输入第二个学生的信息：\n");
	stu2=input_student();						//函数调用 
	printf("第一个学生的信息如下：\n");
	display_student(stu1); 						//函数调用
	printf("第二个学生的信息如下：\n");
	display_student(stu2);						//函数调用
	 
}

struct student input_student()     //输入学生信息的函数 
  {
  	struct student stu;
	printf("姓名:");
	scanf("%s",stu.name);
	printf("性别:");
	scanf("%s",stu.sex);
	printf("年:");
	scanf("%d",&stu.year);
	printf("月:");
	scanf("%d",&stu.mouth);
	printf("日:");
	scanf("%d",&stu.ri);
	return stu; 
  }	
  
void display_student(struct student stu)    //显示学生信息的函数 
  	{
	  	printf("姓名:%s\n",stu.name);
		printf("性别:%s\n",stu.sex);
		printf("年:%d.%d.%d\n",stu.year,stu.mouth,stu.ri);
	}
