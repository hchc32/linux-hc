#include<stdio.h>     
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/epoll.h>
#include<mysql/mysql.h>
#include<assert.h>
#include <termio.h>
#include"my_error.h"
#include"my_socket.h"
#include"meun.h"
#include"user_password.h"
#include"friend.h"
#include"group.h"

#define BUFSIZE 1024
/*初始化链表list。链表为带头结点的双向循环链表*/
#define List_Init(list, list_node_t) {					\
		list=(list_node_t*)malloc(sizeof(list_node_t)); \
		(list)->next=(list)->prev=list;					\
	}
//链表尾插法，list为头指针，new为新节点
#define List_AddTail(list, newNode) {			\
		(newNode)->prev=(list)->prev; 		 	\
		(list)->prev->next=newNode;			 	\
		(newNode)->next=list;				 	\
		(list)->prev=newNode;				 	\
	}

//从删除链表结点node，
#define List_DelNode(node) {\
			assert(NULL!=node && node!=(node)->next && node!=(node)->prev);				\
			(node)->prev->next=(node)->next; 	\
			(node)->next->prev=(node)->prev;	\
	}

//从链表中删除并释放结点node
#define List_FreeNode(node) {	\
		List_DelNode(node);		\
		free(node);				\
	}

char cli_info[10];
char cli_name[20];
//消息链表
Box *list;
//好友链表
Friend *head;
pthread_mutex_t mutex;
pthread_mutex_t flagmutex;
pthread_cond_t cond;
int login_flag;

void *recv_file(void *arg)
{
    printf(".......\n");
    File_Recv *temp = (File_Recv *)arg;
    char path[50];
    char buff[1024];
    sprintf(path,"./%s",temp->filename);
    FILE *fp = fopen(path,"w");
    if(NULL == fp)
    {
        printf("文件出错!\n");
        exit(1);
    }
    int len = 0 ;

    pthread_mutex_lock(&flagmutex);
    bzero(buff,1024);
    while((len = recv(temp->fd,buff,1024,0)) > 0)
    {
        printf("len = %d\n",len);
        if(strcmp(buff,"end\n") == 0)
        {
            break;
        }
        if(fwrite(buff,sizeof(char),len,fp) < len)
        {
            printf("文件接受失败!\n");
            break;
        }
        bzero(buff,1024);
        usleep(1);
    }
    pthread_mutex_unlock(&flagmutex);
    printf("文件接受成功!\n");
    fclose(fp);
    pthread_cond_broadcast(&cond);
    pthread_exit(NULL);
}

void *read_pth(void*arg)
{
    int sfd = (int)arg;
    while(1)
    {
        Data recv_temp;
        memset(&recv_temp,0,sizeof(recv_temp));
        //pthread_mutex_lock(&mutex);
        //pthread_cond_wait(&cond,&mutex);
        if(recv(sfd,&recv_temp,sizeof(recv_temp),0) < 0)
        {
            my_err("recv",__LINE__);
        }
        //pthread_mutex_unlock(&mutex);
        switch(recv_temp.type)
        {
            //注册功能
            case REGIST:{
                            if(strcmp(recv_temp.strings,"no\n") == 0)
                            {
                                printf("注册失败!\n");
                            }
                            else
                            {
                                printf("%s\n",recv_temp.strings);
                            }
                            break;
                        }
            //登录功能
            case LOGIN:{
                            if(strcmp(recv_temp.strings,"no\n") == 0)
                            {
                                printf("登录失败!\n");
                            }
                            else
                            {
                                Log cli_log;
                                memcpy(&cli_log,recv_temp.strings,sizeof(cli_log));
                                strcpy(cli_info,cli_log.accounts);
                                strcpy(cli_name,cli_log.user_name);
                                pthread_mutex_lock(&flagmutex);
                                login_flag = 1;
                                pthread_mutex_unlock(&flagmutex);
                                printf("%s登录成功!",cli_log.accounts);
                            }
                            break;
                           

                       }
            case ADDFRIEND:{
                                //加好友
                               if(strcmp(recv_temp.strings,"no\n") != 0) 
                               {
                                   printf("你有一条验证消息!\n");
                               }
                               else
                               {
                                   printf("没有该帐号!\n");
                               }
                               break;
                            }
            case FRIENDLIST:{
                                Friend_data temp;
                                memset(&temp,0,sizeof(temp));
                                if(strcmp(recv_temp.strings,"no\n") != 0)
                                {
                                    memcpy(&temp,recv_temp.strings,sizeof(temp));
                                }
                                else
                                {
                                    break;
                                }
                                printf("%10s %15s %5d\n" ,temp.accounts,temp.user_name,temp.flag);
                                break;
                            }
            case DELFRIEND:
                            {
                                if(strcmp(recv_temp.strings,"no\n") != 0)
                                {
                                    printf("删除成功!\n");
                                }
                                else
                                {
                                    printf("删除失败!\n");
                                }
                                break;
                            }
            case LOOKFRIEND:{
                                Friend_data temp;
                                if(strcmp(recv_temp.strings,"no\n") != 0)
                                {
                                    memcpy(&temp,recv_temp.strings,sizeof(temp));
                                    printf("帐号:%s\n",temp.accounts);
                                    printf("昵称:%s\n",temp.user_name);
                                    printf("在线状态:%d\n",temp.flag);
                                }
                                else
                                {
                                    printf("查找失败!\n");
                                }
                                break;
                            }
            case DATABOX:{      //查看消息盒子
                                //接受消息存在消息链表中
                                Box *newnode = (Box*)malloc(sizeof(Box));
                                if(strcmp(recv_temp.strings,"no\n") != 0)
                                {
                                    memcpy(&newnode->data,recv_temp.strings,sizeof(newnode->data));
                                }
                                else
                                {
                                    break;
                                }
                                //if(newnode->data.data_type == 30)
                                {
                                    printf("%s->%s: %s\n",newnode->data.s_accounts,\
                                        newnode->data.r_accounts,newnode->data.data_string);
                                }
                                List_AddTail(list,newnode);                            
                                printf("消息读取完成!\n");
                                break;
                        }
            //私聊
            case PRIVCHAT:{
                              s_data temp;
                              memcpy(&temp,&recv_temp.strings,sizeof(temp));
                              //在私聊页面,接受的消息直接打印出来
                              if(temp.data_type == 30)
                              {
                                  printf("%s\n",recv_temp.strings);
                                    printf("%s: %s\n",temp.s_accounts,temp.data_string);
                              }
                              break;
                          }
            //创建群
            case CREATGROUP:{
                                Regist_Group send_temp;
                                if(strcmp(recv_temp.strings,"no\n") != 0)
                                {
                                    memcpy(&send_temp,recv_temp.strings,sizeof(send_temp));
                                    printf("群注册成功!\n");
                                    printf("群号为%s\n",send_temp.group_num);
                                }
                                else
                                {
                                    printf("群注册失败!\n");
                                }
                                break;
                            }
            case DELGROUP:{
                              if(strcmp(recv_temp.strings,"no\n") != 0)
                              {
                                  printf("群号为%s的群解散成功!\n",recv_temp.strings);
                              }
                              else
                              {
                                  printf("群解散失败!\n");
                              }
                              break;
                          }
            case ADDGROUP:{
                              if(strcmp(recv_temp.strings,"no\n") ==0 )
                              {
                                  printf("该群不存在!\n");
                              }
                              else if(strcmp(recv_temp.strings,"yes\n") == 0)
                              {
                                  printf("加群消息发送成功!\n");
                              }
                              else
                              {
                                  printf("%s\n",recv_temp.strings);
                              }
                              break;
                          }
            case LOOKGROUP:{
                                Regist_Group temp;
                                if(strcmp(recv_temp.strings,"no\n") ==0 )
                                {
                                    printf("查询出错!\n");
                                }
                                else if(strcmp(recv_temp.strings,"end\n") == 0)
                                {
                                    break;
                                }
                                else
                                {
                                    memcpy(&temp,recv_temp.strings,sizeof(temp));
                                    printf("     %s     %s     %s         %s\n",temp.group_num,\
                                           temp.group_name,temp.group_owner_accounts,temp.group_owner_name);
                                }
                                break;
                           }
            case LOOKGMEMBER:{
                                Add_Group temp;
                                if(strcmp(recv_temp.strings,"no\n") == 0)
                                {
                                    printf("查询出错!\n");
                                }
                                else if(strcmp(recv_temp.strings,"end\n") == 0)
                                {
                                    break;
                                }
                                else
                                {
                                    memcpy(&temp,recv_temp.strings,sizeof(temp));
                                    printf("%s %s %d\n",temp.accounts,temp.user_name,temp.group_member_flag);
                                }
                                break;
                             }
            case GROUPMEMBER:{
                                Manage temp;
                                memset(&temp,0,sizeof(temp));
                                printf("%s\n",recv_temp.strings);
                                break;
                             }
            case DEMEMBER:{
                                if(strcmp(recv_temp.strings,"no\n") == 0)
                                {
                                    printf("删除出错!\n");
                                }
                                else if(strcmp(recv_temp.strings,"yes\n") == 0)
                                {
                                    printf("删除成功!\n");
                                }
                                else
                                {
                                    printf("%s\n",recv_temp.strings);
                                }
                                break;
                             }
            case SENDFILE:{
                                if(strcmp(recv_temp.strings,"yes\n") == 0)
                                {
                                    printf("文件传输成功!\n");
                                }
                                else
                                {
                                    printf("文件传送失败!\n");
                                }
                                break;
                          }
            case RECVFILE:{
                                if(strcmp(recv_temp.strings,"no\n") == 0)
                                {
                                    printf("文件下载失败!\n");
                                }
                                else
                                {
                                    File_Recv temp;
                                    strcpy(temp.filename,recv_temp.strings);
                                    printf("%s文件正在下载!\n",temp.filename);
                                    pthread_t filepth;
                                    temp.fd = sfd;
                                    pthread_create(&filepth,NULL,recv_file,(void*)&temp);
                                    pthread_mutex_lock(&flagmutex);
                                    pthread_cond_wait(&cond,&flagmutex);
                                    pthread_mutex_unlock(&flagmutex);
                                   // pthread_detach(filepth);
                                }
                               break;
                          }
            //退出系统
            case EXIT:{
                          if(strcmp(recv_temp.strings,"yes\n") == 0)
                          {
                              printf("系统即将退出!\n");
                              exit(1);
                          }
                          break;
                      }
            //找回密码
            case FINDPASS:{
                            char recv_buf[20];
                            if(strcmp(recv_temp.strings,"no\n") == 0)
                            {
                                printf("信息填写错误!\n");
                            }
                            else
                            {
                                printf("你的密码为:%s\n",recv_buf);
                            }
                            break;
                          }
        }
    }
}


void *write_pth(void*arg)
{
    int sfd = (int)arg;
    Regist client_regist;
    while(1)
    {
        int select = 0;
        Log_In_Menu();
        printf("请输入你要操作的功能:");
        scanf("%d",&select);
        switch(select)
        {
            //注册功能
            case REGIST:{
                            memset(&client_regist,0,sizeof(client_regist));
                            printf("请输入你要注册的帐号昵称:\n");
                            scanf("%s",client_regist.user_name);
                            do
                            {
                                printf("请输入你的密码(不小于7位字符):\n");
                                scanf("%s",client_regist.password);
                            }
                            while(strlen(client_regist.password) < 7);
                            printf("密保问题:\n");
                            printf("你的电话号码是多少?(11位数)\n:");
                            scanf("%s",client_regist.phone_num);
                            Data send_string;
                            memset(&send_string,0,sizeof(Data));
                            send_string.type = 1;
                            //结构体转成字符串
                            memcpy(send_string.strings,&client_regist,sizeof(send_string.strings));
                            //向服务器发送数据
                            pthread_mutex_lock(&mutex);
                            if(send(sfd,&send_string,sizeof(send_string),0) < 0)
                            {
                                my_err("send",__LINE__);
                                exit(1);
                            }
                            pthread_mutex_unlock(&mutex);
                            pthread_cond_broadcast(&cond);
                            break;
                        }
            //登录功能
            case LOGIN:{
                        getchar();
                        Data temp;
                        Log input;
                        memset(&temp,0,sizeof(temp));
                        memset(&input,'\0',sizeof(input));
                        printf("帐号:\n");
                        scanf("%s",input.accounts);
                        printf("密码:\n");
                        int i = 0;
                        getchar();
                        while( 1 )
                        {
                            input.password[i] = getch();
                            if(input.password[i] =='\r')
                            {
                                input.password[i] = '\0';
                                break;
                            }
                            if(input.password[i] == 0x7f)
                            {
                                --i;
                                printf("\b \b");        //用空格代替* 
                                continue;
                            }
                            putchar('*');
                            ++i;
                        }
                        temp.type = LOGIN;
                        memcpy(temp.strings,&input,sizeof(temp.strings));
                        pthread_mutex_lock(&mutex);
                        if(send(sfd,&temp,sizeof(temp),0) < 0)
                        {
                            my_err("send",__LINE__);
                        }
                        pthread_mutex_unlock(&mutex);
                        pthread_cond_broadcast(&cond);
                        sleep(1);
                        pthread_mutex_lock(&flagmutex);
                        if(login_flag == 0)
                        {
                            continue;
                        }
                        pthread_mutex_unlock(&flagmutex);
                        while(1)
                        {
                            getchar();
                            Fun_Menu();
                            int select;
                            printf("请输入你的选项:");
                            scanf("%d",&select);
                            switch(select+4)
                            {
                                //加好友
                                case ADDFRIEND:{
                                                    Data send_temp;
                                                    Addfriend temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    printf("请输入你要添加的好友帐号:");
                                                    scanf("%s",temp.friend_accounts);
                                                    strcpy(temp.my_accounts,cli_info);
                                                    send_temp.type = ADDFRIEND;
                                                    memcpy(&send_temp.strings,&temp,sizeof(send_temp.strings));
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&send_temp,sizeof(send_temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    else
                                                    {
                                                        printf("验证消息发送成功!\n");
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    break;
                                                }
                                case FRIENDAPPLY:{//加好友申请
                                                    Addfriend *temp = (Addfriend*)malloc(sizeof(Addfriend));
                                                    Data send_data;
                                                    Box *p = list;
                                                    while(p->next != list)
                                                    {
                                                        p = p->next;
                                                        memset(temp,0,sizeof(Addfriend));
                                                        memset(&send_data,0,sizeof(Data));
                                                        //是好友验证消息
                                                        if(p->data.data_type == ADDF)
                                                        {
                                                            //填充验证加好友包
                                                            temp->f_g_flag = ADDF;
                                                            strcpy(temp->my_accounts,cli_info);
                                                            strcpy(temp->friend_accounts,p->data.s_accounts);
                                                            printf("%s ->  %s\n",cli_info,p->data.s_accounts);
                                                            printf("%s请求加你为好友(y->同意,n->拒绝)\n",p->data.s_accounts);
                                                            getchar();
                                                            scanf("%c",&temp->flag);
                                                            //getchar();
                                                            send_data.type = FRIENDAPPLY;
                                                            memcpy(send_data.strings,temp,sizeof(send_data.strings));
                                                            //发送确认好友包
                                                            //pthread_mutex_lock(&mutex);
                                                            if(send(sfd,&send_data,sizeof(send_data),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            //pthread_mutex_unlock(&mutex);
                                                            //pthread_cond_broadcast(&cond);
                                                            List_FreeNode(p);
                                                            p = p->next;
                                                        }
                                                        else if(p->data.data_type == ADDFRETURN) //添加好友回执包
                                                        {
                                                            //printf("%s\n",p->data.data_string);
                                                        }
                                                        else if(p->data.data_type == ADDG)
                                                        {
                                                            //填充验证加好友包
                                                            strcpy(temp->my_accounts,p->data.data_string);
                                                            strcpy(temp->friend_accounts,p->data.s_accounts);
                                                            printf("%s ->  %s\n",cli_info,p->data.s_accounts);
                                                            printf("%s请求加群%s(y->同意,n->拒绝)\n",\
                                                                   p->data.s_accounts,p->data.data_string);
                                                            getchar();
                                                            scanf("%c",&temp->flag);
                                                            //getchar();
                                                            send_data.type = FRIENDAPPLY;
                                                            memcpy(send_data.strings,temp,sizeof(send_data.strings));
                                                            //发送确认好友包
                                                            //pthread_mutex_lock(&mutex);
                                                            if(send(sfd,&send_data,sizeof(send_data),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            //pthread_mutex_unlock(&mutex);
                                                            //pthread_cond_broadcast(&cond);
                                                            List_FreeNode(p);
                                                            p = p->next;
                                                        }
                                                    }
                                                    free(temp);
                                                    break;
                                                 }
                                case FRIENDLIST:{
                                                    //获取好友列表
                                                    Data temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    temp.type = FRIENDLIST;
                                                    strcpy(temp.strings,cli_info);
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    else
                                                    {
                                                        printf("----------------------好友列表---------------------\n");
                                                        printf("---------帐号----------昵称---------在线状态-------\n");
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                                }
                                case DELFRIEND:
                                                {
                                                    Data temp;
                                                    Addfriend send_temp;
                                                    temp.type = DELFRIEND;
                                                    printf("请输入你要删除的帐号:");
                                                    scanf("%s",send_temp.friend_accounts);
                                                    strcpy(send_temp.my_accounts,cli_info);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                                }
                                case LOOKFRIEND:{
                                                    Data temp;
                                                    printf("请输入要查找的帐号:");
                                                    scanf("%s",temp.strings);
                                                    temp.type = LOOKFRIEND;
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                                }

                                case DATABOX:{      //查看消息盒子
                                                    //向服务器请求，消息表中的消息
                                                    Data temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    //填充帐号
                                                    strcpy(temp.strings,cli_info);
                                                    temp.type = DATABOX;
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                            }
                                //私聊
                                case PRIVCHAT:{
                                                  Box *p = list;
                                                  Data temp;
                                                  s_data send_temp;
                                                  char buf[200];
                                                  printf("请输入你要私聊的帐号:");
                                                  scanf("%s",send_temp.r_accounts);
                                                  //进入私聊页面
                                                  do
                                                  {
                                                        printf("请输入你要发送的消息:");
                                                        scanf("%s",buf);
                                                        send_temp.data_type = MESSAGE;
                                                        strcpy(send_temp.data_string,buf);
                                                        strcpy(send_temp.s_accounts,cli_info);
                                                        temp.type = PRIVCHAT;
                                                        memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                        if(strcmp(buf,"&quit") != 0)
                                                        {
                                                            pthread_mutex_lock(&mutex);
                                                            if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            pthread_mutex_unlock(&mutex);
                                                            pthread_cond_broadcast(&cond);
                                                        }
                                                  }
                                                  while(strcmp(buf,"&quit") != 0);
                                                  break;
                                              }
                                //群聊
                                case GROUPCHAT:
                                              {
                                                  Box *p = list;
                                                  Data temp;
                                                  s_data send_temp;
                                                  char buf[200];
                                                  printf("请输入你要群聊的帐号:");
                                                  scanf("%s",send_temp.r_accounts);
                                                  //进入群聊页面
                                                  do
                                                  {
                                                        printf("请输入你要发送的消息:");
                                                        scanf("%s",buf);
                                                        send_temp.data_type = MESSAGE;
                                                        strcpy(send_temp.data_string,buf);
                                                        strcpy(send_temp.s_accounts,cli_info);
                                                        temp.type = PRIVCHAT;
                                                        memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                        if(strcmp(buf,"&quit") != 0)
                                                        {
                                                            pthread_mutex_lock(&mutex);
                                                            if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            pthread_mutex_unlock(&mutex);
                                                            pthread_cond_broadcast(&cond);
                                                        }
                                                  }
                                                  while(strcmp(buf,"&quit") != 0);
                                                  break;
                                              }
                                //创建群
                                case CREATGROUP:{
                                                    Data temp;
                                                    Regist_Group send_temp;
                                                    printf("请输入你要创建的群昵称:");
                                                    scanf("%s",send_temp.group_name);
                                                    strcpy(send_temp.group_owner_accounts,cli_info);
                                                    strcpy(send_temp.group_owner_name,cli_name);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    temp.type = CREATGROUP;
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                                }
                                case DELGROUP:{
                                                  Data temp;
                                                  Del_Group send_temp;
                                                  printf("请输入你要删除的群号:");
                                                  scanf("%s",send_temp.group_num);
                                                  strcpy(send_temp.accounts,cli_info);
                                                  temp.type = DELGROUP;
                                                  memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                  pthread_mutex_lock(&mutex);
                                                  if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                  {
                                                      my_err("send",__LINE__);
                                                  }
                                                  pthread_mutex_unlock(&mutex);
                                                  pthread_cond_broadcast(&cond);
                                                  sleep(1);
                                                  break;
                                              }
                                case ADDGROUP:{
                                                  Data temp;
                                                  Add_Group send_temp;
                                                  memset(&send_temp,0,sizeof(send_temp));
                                                  printf("请输入你要加的群号:");
                                                  scanf("%s",send_temp.group_num);
                                                  strcpy(send_temp.accounts,cli_info);
                                                  strcpy(send_temp.user_name,cli_name);
                                                  send_temp.group_member_flag = 3;
                                                  memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                  temp.type = ADDGROUP;
                                                  pthread_mutex_lock(&mutex);
                                                  if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                  {
                                                      my_err("send",__LINE__);
                                                  }
                                                  pthread_mutex_unlock(&mutex);
                                                  pthread_cond_broadcast(&cond);
                                                  sleep(1);
                                                  break;
                                              }
                                case LOOKGROUP:{
                                                    Data temp;
                                                    Regist_Group recv_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    temp.type = LOOKGROUP;
                                                    //填充帐号
                                                    strcpy(temp.strings,cli_info);
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    else
                                                    {
                                                        printf("----------------------群列表---------------------\n");
                                                        printf("-----群号---群昵称----群主昵称------群主昵称-----\n");
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                               }
                                case LOOKGMEMBER:{
                                                    Data temp;
                                                    char group_num[6];
                                                    Add_Group recv_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    printf("请输入群号:");
                                                    scanf("%s",group_num);
                                                    temp.type = LOOKGMEMBER;
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                                 }
                                case GROUPMEMBER:{
                                                    Data temp;
                                                    Manage send_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    memset(&send_temp,0,sizeof(send_temp));
                                                    printf("请输入你要设置管理员的群号:");
                                                    scanf("%s",send_temp.group_num);
                                                    printf("请输入成员的帐号:");
                                                    scanf("%s",send_temp.member_accounts);
                                                    strcpy(send_temp.my_accounts,cli_info);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,(void*)&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                                 }
                                case DEMEMBER:{
                                                    Data temp;
                                                    Manage send_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    memset(&send_temp,0,sizeof(send_temp));
                                                    printf("请输入你要踢人的群号:");
                                                    scanf("%s",send_temp.group_num);
                                                    printf("请输入成员的帐号:");
                                                    scanf("%s",send_temp.member_accounts);
                                                    strcpy(send_temp.my_accounts,cli_info);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,(void*)&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    break;
                                                 }
                                case SENDFILE:{
                                                    Data temp;
                                                    File_Rs  send_temp;
                                                    char path[30];
                                                    char *p1,*p2;
                                                    char buff[1024];
                                                    p1 = path;
                                                    p2 = path+1;
                                                    memset(&send_temp,0,sizeof(send_temp));
                                                    memset(&temp,0,sizeof(temp));
                                                    strcpy(send_temp.s_accounts,cli_info);
                                                    printf("请输入对方的帐号:");
                                                    scanf("%s",send_temp.r_accounts);
                                                    printf("请输入文件所在路径:");
                                                    scanf("%s",path);
                                                    for(int i = 0 ; i < strlen(path) ; i++)
                                                    {
                                                        if(*p2 == '/')
                                                        {
                                                            p1 = p2;
                                                            p2++;
                                                        }
                                                        else
                                                        {
                                                            p2++;
                                                        }
                                                    }   
                                                    int i = 0;
                                                    while(p1 != p2)
                                                    {
                                                        p1++;
                                                        send_temp.filename[i++] = *p1;
                                                    }
                                                    printf("filename %s\n",send_temp.filename);
                                                    FILE *fp = fopen(path,"r");
                                                    if(fp == NULL)
                                                    {
                                                        printf("文件打开出错!\n");
                                                        break;
                                                    }
                                                    else
                                                    {
                                                        temp.type = SENDFILE;
                                                        memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                        pthread_mutex_lock(&mutex);
                                                        if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                        {
                                                            my_err("send",__LINE__);
                                                        }
                                                        bzero(buff,1024);
                                                        int len = 0;
                                                        while((len = fread(buff,sizeof(char),1024,fp)) > 0)
                                                        {
                                                            printf("len = %d\n",len);
                                                            if(send(sfd,buff,sizeof(buff),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            bzero(buff,1024);
                                                            usleep(1);
                                                        }
                                                        sleep(1);
                                                    }
                                                    bzero(buff,1024);
                                                    strcpy(buff,"end\n");
                                                    if(send(sfd,buff,sizeof(buff),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    pthread_cond_broadcast(&cond);
                                                    sleep(1);
                                                    break;
                                              }
                                case RECVFILE:{
                                                    File_Rs send_temp;
                                                    Data temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    strcpy(send_temp.s_accounts,cli_info);
                                                    printf("请输入你要接收的文件名:");
                                                    scanf("%s",send_temp.filename);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    temp.type = RECVFILE;
                                                    pthread_mutex_lock(&mutex);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    pthread_mutex_unlock(&mutex);
                                                    
                                                    pthread_mutex_lock(&flagmutex);
                                                    pthread_cond_wait(&cond,&flagmutex);
                                                    pthread_mutex_unlock(&flagmutex);
                                                    //sleep(1);
                                                    break;
                                              }
                                case EXIT+4:{
                                              Data temp;
                                              strcpy(temp.strings,cli_info);
                                              temp.type = EXIT;
                                              pthread_mutex_lock(&mutex);
                                              if(send(sfd,&temp,sizeof(Data),0) < 0)
                                              {
                                                  my_err("send",__LINE__);
                                              }
                                              pthread_mutex_unlock(&mutex);
                                              pthread_cond_broadcast(&cond);
                                              exit(1);
                                              break;
                                          }
                            }
                        }
                       }
            //找回密码
            case FINDPASS:{
                            getchar();
                            Data temp;
                            FindPass input;
                            char buf[BUFSIZE];
                            memset(&temp,0,sizeof(temp));
                            memset(&input,'\0',sizeof(input));
                            memset(buf,'\0',sizeof(buf));
                            printf("帐号:\n");
                            scanf("%s",input.accounts);
                            printf("密保答案:\n");
                            scanf("%s",input.phone_num);

                            temp.type = FINDPASS;
                            memcpy(temp.strings,&input,sizeof(temp.strings));
                            pthread_mutex_lock(&mutex);
                            if(send(sfd,&temp,sizeof(buf),0) < 0)
                            {
                                my_err("send",__LINE__);
                            }
                           pthread_mutex_unlock(&mutex);
                           pthread_cond_broadcast(&cond);
                           break;
                          }
            case EXIT:{
                          Data temp;
                          strcpy(temp.strings,cli_info);
                          temp.type = EXIT;
                          pthread_mutex_lock(&mutex);
                          if(send(sfd,&temp,sizeof(Data),0) < 0)
                          {
                              my_err("send",__LINE__);
                          }
                          pthread_mutex_unlock(&mutex);
                          pthread_cond_broadcast(&cond);
                          break;
                      }
            default:{
                        printf("请输入正确的选项!\n");
                        getchar();
                        continue;
                    }
        }
    }
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


int main(int argc,char *argv[])
{
    int sfd;
    int serv_prot;
    struct sockaddr_in serv_addr;
    pthread_t pth1;
    pthread_t pth2;
    List_Init(list,Box);
    pthread_mutex_init(&mutex,NULL);
    pthread_mutex_init(&flagmutex,NULL);
    pthread_cond_init(&cond,NULL);                
    //检查参数个数
    if(argc < 5)
    {
        printf("./a.out -p serv_prot -a serv_address\n");
        exit(1);
    }

    //初始化服务器端地址结构
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    
    for(int i = 0; i < argc ; i++)
    {
        //命令行获取端口
        if(strcmp(argv[i],"-p") == 0)
        {
            serv_prot = atoi(argv[i+1]);
            if(serv_prot < 0 || serv_prot > 65535)
            {
                printf("无效的prot!\n");
                exit(1);
            }
            else
            {
                serv_addr.sin_port = htons(serv_prot);
            }
        }

        //命令行获取ip地址
        if(strcmp(argv[i],"-a") == 0)
        {
            if(inet_aton(argv[i+1],&serv_addr.sin_addr) == 0)
            {
                printf("无效的ip地址!\n");
                exit(1);
            }
            continue;
        }
    }

    //创建套接字
    sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd < 0)
    {
        my_err("socket",__LINE__);
        exit(1);
    }
    
    //向服务器端请求建立连接
    if(connect(sfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0)
    {
        my_err("connect",__LINE__);
        exit(1);
    }
    //用fsd和服务器通信
    pthread_create(&pth2,NULL,write_pth,(void*)sfd);
    pthread_create(&pth1,NULL,read_pth,(void*)sfd);
    pthread_join(pth1,NULL);
    pthread_join(pth2,NULL);
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&flagmutex);
    pthread_cond_destroy(&cond);
    return 0;
}

    /*
    Regist client_regist;
    while(1)
    {
        int select = 0;
        Log_In_Menu();
        printf("请输入你要操作的功能:");
        scanf("%d",&select);
        switch(select)
        {
            case REGIST:{
                        memset(&client_regist,0,sizeof(client_regist));
                        printf("请输入你要注册的帐号昵称:\n");
                        scanf("%s",client_regist.user_name);
                        do
                        {
                            printf("请输入你的密码(不小于7位字符):\n");
                            scanf("%s",client_regist.password);
                        }
                        while(strlen(client_regist.password) < 7);

                        printf("密保问题:\n");
                        printf("你的电话号码是多少?(11位数)\n:");
                        scanf("%s",client_regist.phone_num);

                        Data send_string;
                        memset(&send_string,0,sizeof(Data));
                        send_string.type = 1;
                        //结构体转成字符串
                        memcpy(send_string.strings,&client_regist,sizeof(send_string.strings));
                        
                        //向服务器发送数据
                        if(send(sfd,&send_string,sizeof(send_string),0) < 0)
                        {
                            my_err("send",__LINE__);
                            exit(1);
                        }

                        //接受服务器端的数据
                        int recv_len = 0;
                        char recv_buf[10];
                        
                        if((recv_len = recv(sfd,recv_buf,sizeof(recv_buf),0)) < 0)
                        {
                            my_err("recv",__LINE__);
                            exit(1);
                        }
                        printf("你的帐号注册成功,请牢记你的帐号!\n");
                        printf("帐号为:%s\n",recv_buf);
                        break;
                   }
            //登录功能
            case LOGIN:{
                        getchar();
                        Log temp = login_userinfo(sfd);
                        if(temp.accounts[0] == 0)
                        {
                            printf("帐号或密码错误!");
                            break;
                        }
                        strcpy(cli_info,temp.accounts);
                        strcpy(cli_name,temp.user_name);
                        printf("%s登录成功!\n",cli_info);
                        List_Init(list,Box);
                        pthread_t pth;
                        pthread_create(&pth,NULL,read1,(void*)sfd);
                        pthread_detach(pth);
                        //登录成功后开一个线程，用来收数据，并存到消息链表中
                        while(1)
                        {
                            Fun_Menu();
                            int select;
                            printf("请输入你的选项:");
                            scanf("%d",&select);
                            switch(select+4)
                            {
                                case ADDFRIEND:{
                                                    //加好友
                                                    int ret;
                                                    if((ret = add_friend(sfd,cli_info)) == 1)
                                                    {
                                                        printf("加好友消息发送成功!\n");
                                                    }
                                                    else if(ret == 0)
                                                    {
                                                        printf("加好友消息发送失败!\n");
                                                    }
                                                    else
                                                    {
                                                        printf("该帐号不存在!\n");
                                                    }
                                                    break;
                                                }
                                case FRIENDAPPLY:{//加好友申请
                                                    Addfriend *temp = (Addfriend*)malloc(sizeof(Addfriend));
                                                    Data send_data;
                                                    //= (Data*)malloc(sizeof(Data));
                                                    Box *p = list;
                                                    printf("%d\n",__LINE__);
                                                    while(p->next != list)
                                                    {
                                                        p = p->next;
                                                        printf("%d\n",__LINE__);
                                                        memset(temp,0,sizeof(Addfriend));
                                                        memset(&send_data,0,sizeof(Data));
                                                        //是好友验证消息
                                                        if(p->data.data_type == ADDF)
                                                        {
                                                            //填充验证加好友包
                                                            strcpy(temp->my_accounts,cli_info);
                                                            strcpy(temp->friend_accounts,p->data.s_accounts);
                                                            printf("%s ->  %s\n",cli_info,p->data.s_accounts);
                                                            printf("%s请求加你为好友(y->同意,n->拒绝)\n",p->data.s_accounts);
                                                            getchar();
                                                            scanf("%c",&temp->flag);
                                                            //getchar();
                                                            send_data.type = FRIENDAPPLY;
                                                            memcpy(send_data.strings,temp,sizeof(send_data.strings));
                                                            //发送确认好友包
                                                            if(send(sfd,&send_data,sizeof(send_data),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            printf("%d\n",send_data.type);
                                                            List_FreeNode(p);
                                                            p = p->next;
                                                        }
                                                        else if(p->data.data_type == ADDFRETURN) //添加好友回执包
                                                        {
                                                            printf("%s\n",p->data.data_string);
                                                        }
                                                        else if(p->data.data_type == ADDG)
                                                        {
                                                            //填充验证加好友包
                                                            strcpy(temp->my_accounts,p->data.data_string);
                                                            strcpy(temp->friend_accounts,p->data.s_accounts);
                                                            printf("%s ->  %s\n",cli_info,p->data.s_accounts);
                                                            printf("%s请求加群%s(y->同意,n->拒绝)\n",\
                                                                   p->data.s_accounts,p->data.data_string);
                                                            getchar();
                                                            scanf("%c",&temp->flag);
                                                            //getchar();
                                                            send_data.type = FRIENDAPPLY;
                                                            memcpy(send_data.strings,temp,sizeof(send_data.strings));
                                                            //发送确认好友包
                                                            if(send(sfd,&send_data,sizeof(send_data),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            printf("%d\n",send_data.type);
                                                            List_FreeNode(p);
                                                            p = p->next;
                                                        }
                                                    }
                                                    free(temp);
                                                    //free(send_data);
                                                    break;
                                                 }
                                case FRIENDLIST:{
                                                    //获取好友列表
                                                    Data temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    temp.type = FRIENDLIST;
                                                    strcpy(temp.strings,cli_info);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    int count;
                                                    //接受好友个数
                                                    if(recv(sfd,&count,sizeof(count),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    printf("count:%d\n",count);
                                                    List_Init(head,Friend);
                                                    for(int i = 0 ; i < count ; i++)
                                                    {
                                                        memset(&temp,0,sizeof(temp));
                                                        Friend *newnode = (Friend*)malloc(sizeof(Friend));
                                                        if(recv(sfd,&temp,sizeof(temp),0) < 0)
                                                        {
                                                            my_err("recv",__LINE__);
                                                        }
                                                        memcpy(&newnode->data,temp.strings,sizeof(newnode->data));
                                                        List_AddTail(head,newnode);
                                                    }
                                                    Friend *cur = head;
                                                    printf("----------------------好友列表---------------------\n");
                                                    printf("---------帐号----------昵称---------在线状态-------\n");
                                                    for(int i = 0; i < count ; i++)
                                                    {
                                                        cur = cur->next;
                                                        printf("%10s %15s %3d\n" ,cur->data.accounts,cur->data.user_name,\
                                                               cur->data.flag);
                                                    }
                                                    break;
                                                }
                                case DATABOX:{      //查看消息盒子
                                                    //向服务器请求，消息表中的消息
                                                    Data temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    //填充帐号
                                                    strcpy(temp.strings,cli_info);
                                                    temp.type = DATABOX;
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    int count = 0;
                                                    //接收消息的个数
                                                    if(recv(sfd,&count,sizeof(count),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    printf("count:%d\n",count);
                                                    //接受消息存在消息链表中
                                                    for(int i = 0 ; i < count ; i++)
                                                    {
                                                        
                                                        memset(&temp,0,sizeof(temp));
                                                        Box *newnode = (Box*)malloc(sizeof(Box));
                                                        //数据：Ｄａｔａ
                                                        if(recv(sfd,&temp,sizeof(temp),0) < 0)
                                                        {
                                                            my_err("recv",__LINE__);
                                                        }
                                                        memcpy(&newnode->data,temp.strings,sizeof(newnode->data));
                                                        printf("%s->%s: %s\n",newnode->data.s_accounts,\
                                                               newnode->data.r_accounts,newnode->data.data_string);
                                                        List_AddTail(list,newnode);                            
                                                    }
                                                    printf("消息读取完成!\n");
                                                    break;
                                            }
                                //私聊
                                case PRIVCHAT:{
                                                  Box *p = list;
                                                  Data temp;
                                                  s_data send_temp;
                                                  char buf[200];
                                                  printf("请输入你要私聊的帐号:");
                                                  scanf("%s",send_temp.r_accounts);
                                                  //进入私聊页面
                                                  do
                                                  {
                                                        printf("请输入你要发送的消息:");
                                                        scanf("%s",buf);
                                                        send_temp.data_type = 30;
                                                        strcpy(send_temp.data_string,buf);
                                                        strcpy(send_temp.s_accounts,cli_info);
                                                        temp.type = PRIVCHAT;
                                                        memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                        if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                        {
                                                            my_err("send",__LINE__);
                                                        }
                                                        //在私聊页面,接受的消息直接打印出来
                                                        if(p->data.data_type == 30)
                                                        {
                                                            printf("%s: %s\n",p->data.s_accounts,p->data.data_string);
                                                        }
                                                  }
                                                  while(buf[0] != '0');
                                                  break;
                                              }
                                //创建群
                                case CREATGROUP:{
                                                    Data temp;
                                                    Regist_Group send_temp;
                                                    char recv_buf[6];
                                                    printf("请输入你要创建的群昵称:");
                                                    scanf("%s",send_temp.group_name);
                                                    strcpy(send_temp.group_owner_accounts,cli_info);
                                                    strcpy(send_temp.group_owner_name,cli_name);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    temp.type = CREATGROUP;
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    if(recv(sfd,recv_buf,sizeof(recv_buf),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    if(recv_buf[0] != INVALID_USERINFO)
                                                    {
                                                        printf("群注册成功!\n");
                                                        printf("群号为%s\n",recv_buf);
                                                    }
                                                    else
                                                    {
                                                        printf("群注册失败!\n");
                                                    }
                                                    break;
                                                }
                                case DELGROUP:{
                                                  Data temp;
                                                  Del_Group send_temp;
                                                  char recv_buf[6];
                                                  memset(recv_buf,0,sizeof(recv_buf));
                                                  printf("请输入你要删除的群号:");
                                                  scanf("%s",send_temp.group_num);
                                                  strcpy(send_temp.accounts,cli_info);
                                                  temp.type = DELGROUP;
                                                  memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                  if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                  {
                                                      my_err("send",__LINE__);
                                                  }
                                                  if(recv(sfd,recv_buf,sizeof(recv_buf),0) < 0)
                                                  {
                                                      my_err("recv",__LINE__);
                                                  }
                                                  if(recv_buf[0] == VALID_USERINFO)
                                                  {
                                                      printf("群号为%s的群解散成功!\n",send_temp.group_num);
                                                  }
                                                  else
                                                  {
                                                      printf("群解散失败!\n");
                                                  }
                                                  break;
                                              }
                                case ADDGROUP:{
                                                  Data temp;
                                                  Add_Group send_temp;
                                                  char recv_buf[6];
                                                  memset(recv_buf,0,sizeof(recv_buf));
                                                  memset(&send_temp,0,sizeof(send_temp));
                                                  printf("请输入你要加的群号:");
                                                  scanf("%s",send_temp.group_num);
                                                  strcpy(send_temp.accounts,cli_info);
                                                  strcpy(send_temp.user_name,cli_name);
                                                  send_temp.group_member_flag = 3;
                                                  memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                  temp.type = ADDGROUP;
                                                  if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                  {
                                                      my_err("send",__LINE__);
                                                  }
                                                  if(recv(sfd,recv_buf,sizeof(recv_buf),0) < 0)
                                                  {
                                                      my_err("recv",__LINE__);
                                                  }
                                                  if(recv_buf[0] == INVALID_USERINFO)
                                                  {
                                                      printf("该群不存在!\n");
                                                  }
                                                  else
                                                  {
                                                      printf("加群消息发送成功!\n");
                                                  }
                                                  break;
                                              }
                                case LOOKGROUP:{
                                                    Data temp;
                                                    Regist_Group recv_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    temp.type = LOOKGROUP;
                                                    //填充帐号
                                                    strcpy(temp.strings,cli_info);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    int count = 0;
                                                    //接收消息的个数
                                                    if(recv(sfd,&count,sizeof(count),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    printf("群列表如下:\n");
                                                    for(int i = 0 ; i < count ; i++)
                                                    {
                                                        
                                                        memset(&temp,0,sizeof(temp));
                                                        if(recv(sfd,&temp,sizeof(temp),0) < 0)
                                                        {
                                                            my_err("recv",__LINE__);
                                                        }
                                                        memcpy(&recv_temp,temp.strings,sizeof(recv_temp));
                                                        printf("%s %s %s %s\n",recv_temp.group_num,\
                                                               recv_temp.group_name,recv_temp.group_owner_accounts,recv_temp.group_owner_name);
                                                    }
                                                    printf("群列表读取完成!\n");
                                                    break;
                                               }
                                case LOOKGMEMBER:{
                                                    Data temp;
                                                    char group_num[6];
                                                    Add_Group recv_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    printf("请输入群号:");
                                                    scanf("%s",group_num);
                                                    temp.type = LOOKGMEMBER;
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    int count = 0;
                                                    //接收消息的个数
                                                    if(recv(sfd,&count,sizeof(count),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    if(count == -1)
                                                    {
                                                        printf("该群不存在!\n");
                                                    }
                                                    printf("%s群的成员如下:\n",group_num);
                                                    for(int i = 0 ; i < count ; i++)
                                                    {
                                                        memset(&recv_temp,0,sizeof(recv_temp));
                                                        memset(&temp,0,sizeof(temp));
                                                        if(recv(sfd,&temp,sizeof(temp),0) < 0)
                                                        {
                                                            my_err("recv",__LINE__);
                                                        }
                                                        memcpy(&recv_temp,temp.strings,sizeof(recv_temp));
                                                        printf("%s %s %d\n",recv_temp.accounts,recv_temp.user_name,recv_temp.group_member_flag);
                                                    }
                                                    printf("群成员显示完成!");
                                                    break;
                                                 }
                                case GROUPMEMBER:{
                                                    Data temp;
                                                    Manage send_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    memset(&send_temp,0,sizeof(send_temp));
                                                    printf("请输入你要设置管理员的群号:");
                                                    scanf("%s",send_temp.group_num);
                                                    printf("请输入成员的帐号:");
                                                    scanf("%s",send_temp.member_accounts);
                                                    strcpy(send_temp.my_accounts,cli_info);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    if(send(sfd,(void*)&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    memset(&temp,0,sizeof(temp));
                                                    if(recv(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    printf("%s\n",temp.strings);
                                                    break;
                                                 }
                                case DEMEMBER:{
                                                    Data temp;
                                                    Manage send_temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    memset(&send_temp,0,sizeof(send_temp));
                                                    printf("请输入你要踢人的群号:");
                                                    scanf("%s",send_temp.group_num);
                                                    printf("请输入成员的帐号:");
                                                    scanf("%s",send_temp.member_accounts);
                                                    strcpy(send_temp.my_accounts,cli_info);
                                                    memcpy(&temp.strings,&send_temp,sizeof(temp.strings));
                                                    if(send(sfd,(void*)&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    memset(&temp,0,sizeof(temp));
                                                    if(recv(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    printf("%s\n",temp.strings);
                                                    break;
                                                 }
                                
                                case EXIT+4:{
                                              Data temp;
                                              printf("cli.info%s",cli_info);
                                              strcpy(temp.strings,cli_info);
                                              temp.type = EXIT;
                                              if(send(sfd,&temp,sizeof(Data),0) < 0)
                                              {
                                                  my_err("send",__LINE__);
                                              }
                                              sleep(1);
                                              printf("系统即将退出!\n");
                                              exit(1);
                                              break;
                                          }

                            }
                        }
                        break;
                   }
            //找回密码
            case FINDPASS:{
                           char recv_buf[20];
                           getchar();
                           if(find_userinfo(sfd) == 0)
                           {
                               printf("密保信息错误!");
                               break;
                           }
                           
                           if(recv(sfd,recv_buf,sizeof(recv_buf),0) < 0)
                           {
                               my_err("recv",__LINE__);
                           }
                           printf("你的密码为:%s\n",recv_buf);
                           break;
                          }
            case EXIT:{
                          */
