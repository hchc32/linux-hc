#ifndef _GROUP_H_
#define _GROUP_H_
//群结构体

#define CREATGROUP  11
#define DELGROUP    12
#define ADDGROUP    13
#define LOOKGROUP   14
#define LOOKGMEMBER 15
#define GROUPMEMBER 16
#define DEMEMBER    17



typedef struct qunxinxi
{
    char group_num[6];
    char group_name[20];
    char group_owner_accounts[10];
    char group_owner_name[20];
}Regist_Group;

typedef struct shanchu
{
    char accounts[10];
    char group_num[6];
}Del_Group;

//add_group accounts name 群号
typedef struct jiaqun
{
    char group_num[6];
    char accounts[10];
    char user_name[20];
    int group_member_flag;
}Add_Group;

typedef struct guanliyuan
{
    char my_accounts[10];
    char member_accounts[10];
    char group_num[6];
}Manage;


#endif
