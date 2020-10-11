#ifndef MY_PACK_H
#define MY_PACK_H

    typedef struct 
    {
        int ret_AAA;
        int send_fd;
        int recv_fd;
        int send_account;
        int recv_account;
        char send_user[50];
        char recv_user[50];
        char read_buf[1024];
        char write_buf[1024];
    }DATA;
    typedef struct
    {
        int type;
        DATA data;
    } PACK;
    
    typedef struct 
    {
        int type[1500];                               //接受到的消息的类型
        char message_date[500][30];          //每条消息的发送时间
        char plz_date[500][30];              //好友请求的发送时间
        //1.一般消息      2.文件        3.好友请求        

        int plz_number;                     //申请群聊数
        int plz_group[500];                 //申请群聊账户
        char group_date[500][30];         //申请群聊日期

        int file_number;
        int file_account[500];
        char file_date[500][30];
        char file[500][50];

        int number;                             //接受到的消息总数
        int talk_number;                        //接受到的消息数
        int friend_number;                      //接受到的请求数
        int group_number;                       //接受到的群消息数
        int message_account[500];               //发送消息人的账户
        int plz_account[100];                   //发送好友请求人的账户

        
        char read_buf[500][1024];               //发送的消息
        char write_buf[500][1024];              //发送的请求
        int send_group_account[500];            //发送消息群中人的账户
        char messages[500][1024];               //接收到的消息
        char group_messages[500][1024];         //群发送的消息
        int group_account[500];                 //发送消息的群账户
    }BOX;

typedef struct 
{
    int send_user;
    int recv_user;
    char messages[BUFSIZ];
    int send_can_look;
    int recv_can_look;
    int message_type;
    char date[81];
}mysql_chat_messages;
typedef struct 
{
    int user;
    int friend_user;
    int realtion;
}mysql_friends;
typedef struct 
{
    int group_account;
    char group_name[81];
    int group_number;
}mysql_group_data;
typedef struct 
{
    int group_account;
    char group_name[81];
    int group_member_account ;
    char group_member_nickname[81];
    int group_state;
}mysql_group_member;
typedef struct
{
    int account;
    char nickname[81];
    char password[81];
    int user_state;
    int user_socket;
    char mibao[81];
    char daan[81];
}mysql_user_data;


#endif 