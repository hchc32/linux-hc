#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NOT_EXIST -1

//字典数据结构
typedef struct
{
    //字符串数组
    char **seq;
    //数字数组
    int *code;
    //字典目前的大小
    int size;
    //字典的容量
    int max_size;
}Dictionary;


//读取文件
//往字典插入元素
void Insert_Seq(Dictionary *dict , char *seq)
{
    //获取字典目前的大小
    int i = dict->size;
    dict->seq[i] = (char*)malloc(sizeof(char) * strlen(seq)+1);
    dict->code[i] = i;
    //改变字典目前的大小
    dict->size++;
    //插入字符串
    strcpy(dict->seq[i],seq);
}

//初始化字典
void Init_Dictionary(Dictionary *dict,int max_size)
{
    dict->max_size = max_size;
    dict->size     = 0;
    dict->seq      = (char**)malloc(sizeof(char*) * max_size);
    dict->code     = (int*)malloc(sizeof(int) * max_size);
    //不用０
    Insert_Seq(dict , "#");
    //将26个英文字母插入字典当中去
    char seq[2] = "A";
    for(int i = 0; i < 26; i++)
    {
        Insert_Seq(dict , seq);
        seq[0]++;
    }
}

//查找
int Get_Seq_Code(Dictionary *dict , char *seq)
{
    for(int i = 0; i < dict->size ; i++)
    {
        if(strcmp(dict->seq[i],seq) == 0 )
        {
            return dict->code[i];
        }
    }
    return NOT_EXIST;
}

//打印字典
void Print_Dictionary(Dictionary *dict)
{
    printf("=====================\n");
    printf("Code         Sequence\n");
    printf("=====================\n");
    for(int i = 0; i < dict->size ; i++)
    {
        printf("%5d%9c",dict->code[i], ' ');
        printf("%s\n",dict->seq[i]);
    }
    printf("=====================\n");
}


void Lzw_Encoder(char *text , Dictionary *dict)
{
    char current[1000];
    char current_temp[1000];
    char next;
    int code;
    int i = 0;
    while(i < strlen(text))
    {
        sprintf(current,"%c",text[i]);
        next = text[i+1];
        //字典中没有找到编码
        while( Get_Seq_Code(dict , current) !=  NOT_EXIST )
        {
            //snprintf(current,1002,"%s%c",current,next);
            snprintf(current_temp,1001,"%s%c",current,next); 
            strcpy(current,current_temp);
            i++;
            next = text[i+1];
        }
        if( i < strlen(text) )
        {
            Insert_Seq(dict,current);
            //printf("%s %d \n",current,code);
        }
            current[strlen(current)-1] = '\0';
            code = Get_Seq_Code(dict , current);
            //next = text[i];
            //snprintf(current_temp,1001,"%s%c",current,next); 
            //strcpy(current,current_temp);
            printf("%s %d \n",current,code);
        
    }

}

int main()
{
    Dictionary dict;
    char buf[10000000];
    memset(buf,'\0',sizeof(buf));
    char str[100] = "TOBEORNOTTOBEORTOBEORNOT\0\0\0";
    Init_Dictionary(&dict,1000);
    Print_Dictionary(&dict);
    //printf("%d\n",Get_Seq_Code(&dict,"B"));
    Lzw_Encoder(str,&dict);
    Print_Dictionary(&dict);
    return 0;
}

