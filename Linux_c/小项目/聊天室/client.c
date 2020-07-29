#include<stdio.h>     
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<errno.h>
#include<unistd.h>
#include<sys/epoll.h>
#include<mysql/mysql.h>
#include<assert.h>
#include"my_error.h"
#include"my_socket.h"
#include"meun.h"
#include"user_password.h"
#include"friend.h"

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
//消息链表
Box *list;
//好友链表
Friend *head;


int main(int argc,char *argv[])
{
    int sfd;
    int serv_prot;
    struct sockaddr_in serv_addr;
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
     
    //和服务器通信
    
    //打印初始界面
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
                        char *temp = login_userinfo(sfd);
                        if(temp == NULL)
                        {
                            printf("帐号或密码错误!");
                            break;
                        }
                        strcpy(cli_info,temp);
                        printf("登录成功!\n");
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
                                                    Data *send_data = (Data*)malloc(sizeof(Data));
                                                    Box *p = list->next;
                                                    while(p->next != list)
                                                    {
                                                        memset(temp,0,sizeof(Addfriend));
                                                        memset(send_data,0,sizeof(Data));
                                                        //是好友验证消息
                                                        if(p->data.data_type == ADDF)
                                                        {
                                                            //填充验证加好友包
                                                            strcpy(temp->friend_accounts,cli_info);
                                                            strcpy(temp->my_accounts,p->data.s_accounts);
                                                            send_data->type = FRIENDAPPLY;
                                                            printf("%s请求加你为好友(y->同意,n->拒绝)",p->data.s_accounts);
                                                            scanf("%c",&temp->flag);
                                                            memcpy(send_data->strings,temp,sizeof(send_data->strings));
                                                            //发送确认好友包
                                                            if(send(sfd,send_data,sizeof(send_data),0) < 0)
                                                            {
                                                                my_err("send",__LINE__);
                                                            }
                                                            List_FreeNode(p);
                                                        }
                                                        else if(p->data.data_type == ADDFRETURN) //添加好友回执包
                                                        {
                                                            printf("\n");
                                                        }
                                                    }
                                                    free(temp);
                                                    free(send_data);
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
                                                        printf("%10s %20s %3d\n" ,cur->data.accounts,cur->data.user_name,\
                                                               cur->data.flag);
                                                    }
                                                }
                                case DATABOX:{      //查看消息盒子
                                                    //向服务器请求，消息表中的消息
                                                    Data temp;
                                                    memset(&temp,0,sizeof(temp));
                                                    temp.type = DATABOX;
                                                    //填充帐号
                                                    strcpy(temp.strings,cli_info);
                                                    if(send(sfd,&temp,sizeof(temp),0) < 0)
                                                    {
                                                        my_err("send",__LINE__);
                                                    }
                                                    int count;
                                                    //接收消息的个数
                                                    if(recv(sfd,&count,sizeof(count),0) < 0)
                                                    {
                                                        my_err("recv",__LINE__);
                                                    }
                                                    List_Init(list,Box);
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
                                                        List_AddTail(list,newnode);                            
                                                    }
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
                          Data temp;
                          printf("cli.info%s",cli_info);
                          strcpy(temp.strings,cli_info);
                          temp.type = EXIT;
                          if(send(sfd,&temp,sizeof(Data),0) < 0)
                          {
                              my_err("send",__LINE__);
                          }
                          printf("系统即将退出!\n");
                          sleep(1);
                          exit(1);
                          break;
                      }
            default:{
                        printf("请输入正确的选项!\n");
                        getchar();
                        continue;
                    }
        }
    }

    //进入主界面
    //1.获取好友名单
    //2.私聊
    //群聊
    //加好友
    //删除好友
    //删除群成员
    //加群
    //发文件
    
    return 0;
}

