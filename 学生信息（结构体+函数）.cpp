#include<stdio.h>
#include<conio.h>
struct student input_student();    //����ѧ����Ϣ 
void display_student(struct student stu);    //��ʾѧ����Ϣ 

struct student    //����ṹ�� 
{
	char name[9];
	char sex[3];
	int year,mouth,ri;
	float score;
};
 
main()
{
	struct student stu1,stu2;      				//���ýṹ��
	printf("�������һ��ѧ������Ϣ��\n"); 
	stu1=input_student();               		//�������� 
	printf("������ڶ���ѧ������Ϣ��\n");
	stu2=input_student();						//�������� 
	printf("��һ��ѧ������Ϣ���£�\n");
	display_student(stu1); 						//��������
	printf("�ڶ���ѧ������Ϣ���£�\n");
	display_student(stu2);						//��������
	 
}

struct student input_student()     //����ѧ����Ϣ�ĺ��� 
  {
  	struct student stu;
	printf("����:");
	scanf("%s",stu.name);
	printf("�Ա�:");
	scanf("%s",stu.sex);
	printf("��:");
	scanf("%d",&stu.year);
	printf("��:");
	scanf("%d",&stu.mouth);
	printf("��:");
	scanf("%d",&stu.ri);
	return stu; 
  }	
  
void display_student(struct student stu)    //��ʾѧ����Ϣ�ĺ��� 
  	{
	  	printf("����:%s\n",stu.name);
		printf("�Ա�:%s\n",stu.sex);
		printf("��:%d.%d.%d\n",stu.year,stu.mouth,stu.ri);
	}
