#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<time.h>
#include<mysql.h>
#include<signal.h>
#include<fcntl.h>




/*
    创建一个消息盒子表，
    登录前查看消息盒子表是否有该账户信息，如果有则在底部提示请求查看（pack.type == 16）
    登录时记得设置用户套接字和修改用户状态信息
    挨个查看，挨个处理
    客户端要给足需要传入的参数，
    服务器端进行筛选，最终反馈给mysql数据库
    客户端去反复接受服务器端传入的数据（如需要获取好友列表）
    获取到就printf
    把所有数字改成宏
*/

#include "my_pack.h"
#include "my_sql.h"

int epfd ;
MYSQL mysql;
int ACCOUNT;
int CONN_FD;
char server_buf[4096] = "\0";
int fp;
int sock_fd;
#define PORT 8014
#define MAX_SIZE 1024
#define EPOLLMAX 1024


#define LOGIN 1
#define REGISTERED 2
#define FIND_PASSWORD 3
#define CHANGE_PASSWORD 33
#define EXIT_CUI 4


#define SIGNAL_SEND 101
#define GROUP_CHAT 102

#define ADD_FRIEND 103
#define DEL_FRIEND 104
#define LOOK_FRIEND 105
#define LOOK_CHAT_RECORD 106
#define SET_REALTION 107

#define CREATE_GROUP 108
#define ADD_GROUP 1081
#define RETURN_GROUP 1082
#define OUT_MEMBER 1083
#define FREE_GROUP 109
#define LOOK_GROUP_MEMBER 110
#define LOOK_GROUP 111

#define SET_QUN 112
#define SEND_FILE 113
#define EXIT_ACCOUNT 114

#define LOOK_MESSAGEBOX 115
#define MANAGE_FRIEND 116
#define MANAGE_MESSAGE 117
#define MANAGE_GROUP 118
#define MANAGE_FILE 120
#define RECV_FILE 119
typedef struct Item{
    mysql_chat_messages chat;
    mysql_friends friends;
    mysql_group_data data;
    mysql_group_member member;
    mysql_user_data user;
}Item;

typedef struct mysql_node{
    Item item;
    struct mysql_node *next;
}mysql_node;

mysql_node *head_chat = NULL;
mysql_node *head_friends = NULL;
mysql_node *head_data = NULL;
mysql_node *head_member = NULL;
mysql_node *head_user = NULL;

int Login_Account(PACK *pack,MYSQL *mysql);
int Registed_Account(PACK *pack,MYSQL *mysql);
int Find_Password(PACK *pack,MYSQL *mysql);
int Change_Password(PACK *pack,MYSQL * mysql);
int Exit_Account(PACK *pack,MYSQL *mysql);
void signal_fun(int signo);
int getNowTime(char *nowTime);
int Find_Friend_Realtion(PACK *pack);
int Add_Friends(PACK *pack,MYSQL *mysql);
int Signal_Chat(PACK *pack);
int Find_Account(int account);
int Del_Friend(PACK * pack,MYSQL *mysql);
int Offline_Messages(int fd,int account);
int Look_MessageBox(int fd,int account);
int Look_Friend(int fd,int account);
int Manage_Friend(int recv_account,int send_account);
int Look_Chat_Record(int fd,int account);
int Manage_Message(int send_account,int recv_account,const char * string);
int Set_Realtion(int send_account,int recv_account,int state);
int Registed_Group(PACK *pack);
int Free_Group(int account,int group);
int Look_Group_Member(int fd,int account,int group);
int Look_Group(int fd,int account);
int Return_Group(int account,int group);
int Group_Chat(PACK * pack);
int Add_Group(int send_account,int group);
int Manage_Group(PACK *pack);
int Recv_File(int fd,PACK *pack);
int Send_File(int fd,PACK * pack);
int Manage_File(int send_account,int recv_account,const char *string);
int Set_Qun(PACK *pack);
int Out_Member(int account,int group_account,int accounted);
void Get_Time(char *string);
void Cut_File( char *string,char *filename);

void read_mysql_friends(mysql_node ** phead);
void read_mysql_user_data(mysql_node **phead);
void read_mysql_chat_messages(mysql_node ** phead);
void read_mysql_group_member(mysql_node ** phead);
void read_mysql_group_data(mysql_node ** phead);
void mysql_list(mysql_node ** phead,mysql_node *new_node);

int main()
{
    
    fp = open("server.dat",O_RDWR | O_APPEND);
    if(fp < 0)
    {
        printf("------\n");
        return 0;
    }
     time_t ticks;
     ticks = time(NULL); 
     sprintf(server_buf,"服务器登录时间 ： %s\n\n\n\n",ctime(&ticks));
     write(fp,server_buf,strlen(server_buf));  
    signal(SIGINT,signal_fun);
    accept_mysql(&mysql);
    struct sockaddr_in serv_addr,cli_addr;
    int conn_fd;
    socklen_t cli_len= sizeof(struct sockaddr_in);
    if((sock_fd = socket(AF_INET,SOCK_STREAM,0)) == -1)
        my_err("socket",__LINE__);
    int optval = 1;
    if(setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int))<0)
    {
        perror("setsocket\n");
        exit(1);
    }
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);  // 8014
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&(serv_addr.sin_zero),sizeof(serv_addr.sin_zero));
    
    if(bind(sock_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr_in)) < 0)
        my_err("bind",__LINE__);

    if(listen(sock_fd,MAX_SIZE) == -1)
        my_err("listen",__LINE__);

    epfd = epoll_create(10);
    struct epoll_event events[EPOLLMAX];
    struct epoll_event ev;
    
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = sock_fd;
    
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev) < 0)
        my_err("epoll ctl ",__LINE__);
    
    int nfds = 1,curfds = 1;
    PACK pack;
    cli_len = sizeof(struct sockaddr_in);
    read_mysql_user_data(&head_user);
    read_mysql_chat_messages(&head_chat);
    read_mysql_friends(&head_friends);
    read_mysql_group_data(&head_data);
    read_mysql_group_member(&head_member);
    

    while(1)
    {
        nfds = epoll_wait(epfd,events,curfds,-1);
        if(nfds < 0)
            my_err("epoll wait",__LINE__);
    
        int i = 0;
        for(i = 0;i < nfds;i++)
        {
            if(events[i].data.fd == sock_fd)
            {
                if((conn_fd = accept(sock_fd,(struct sockaddr *)&cli_addr,&cli_len)) < 0)
                    my_err("accept",__LINE__);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_fd;
                if(epoll_ctl(epfd,EPOLL_CTL_ADD,ev.data.fd,&ev) < 0)
                    my_err("epoll ctl",__LINE__);
                time_t ticks;
                ticks = time(NULL);    
                memset(server_buf,0,sizeof(server_buf));
                sprintf(server_buf,"套接字编号为%d连接至服务器\t\t\t\t%.24s\n",conn_fd,ctime(&ticks));
                write(fp,server_buf,strlen(server_buf));
                curfds ++ ;
                continue;
            }
            
            
            if(events[i].events & EPOLLIN)
            {
                memset(&pack,0,sizeof(pack));
                int n = recv(events[i].data.fd,&pack,sizeof(pack),0);
                if( n < 0)
                {
                    close(events[i].data.fd);
                    perror("recv ");
                    continue;
                }
                if(pack.type == LOGIN)
                {
                    int ret_LO = -1;
                    CONN_FD = events[i].data.fd;
                    ret_LO = Login_Account(&pack,&mysql);
                    
                    time_t ticks;
                    ticks = time(NULL);
                    send(events[i].data.fd,&ret_LO,sizeof(ret_LO),0);
                    if(ret_LO == 1)
                    {
                        memset(server_buf,0,sizeof(server_buf));
                        sprintf(server_buf,"账户 : %d登录至服务器\t\t\t\t%.24s\n",ACCOUNT,ctime(&ticks));
                        write(fp,server_buf,strlen(server_buf));
                        sleep(1);
 //..............................        
                         Offline_Messages(events[i].data.fd,pack.data.send_account);               
                        CONN_FD = 0;
                    }
                    continue;
                }
                else if(pack.type == REGISTERED)
                {
                    int ret_RE = -1;
                    ret_RE = Registed_Account(&pack,&mysql);
                    if(send(events[i].data.fd,&ret_RE,sizeof(ret_RE),0) < 0)
                    {
                        printf("send error,%d\n",__LINE__);
                        continue;
                    }
                    

                }
                else if(pack.type == FIND_PASSWORD)
                {
                    int ret_FP = -1;
                    ret_FP = Find_Password(&pack,&mysql);
                    if(ret_FP == 1)
                    {
                        send(events[i].data.fd,&pack,sizeof(pack),0);

                    }
                    else 
                    {

                        continue;
                    }
                }
                else if(pack.type == CHANGE_PASSWORD)
                {

                    int ret_CP = -1;
                    ret_CP = Change_Password(&pack,&mysql);
                    send(events[i].data.fd,&ret_CP,sizeof(ret_CP),0);


                }
                else if(pack.type == EXIT_CUI)
                {
                    curfds --;
                    time_t ticks;
                    ticks = time(NULL);
                    if(epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&ev) < 0)
                        my_err("epoll ctl",__LINE__);
                    memset(server_buf,0,sizeof(server_buf));

                    sprintf(server_buf,"套接字编号为%d退出服务器\t\t\t\t\t%.24s\n",events[i].data.fd,ctime(&ticks));
                    write(fp,server_buf,strlen(server_buf));
                    close(events[i].data.fd);

                    continue;
                }
                else if(pack.type == EXIT_ACCOUNT)
                {

                    
                    time_t ticks;
                    ticks = time(NULL);
                    int ret_EA = -1;
                    memset(server_buf,0,sizeof(server_buf));
                    sprintf(server_buf,"账户：%d 退出服务器\t\t\t\t%.24s\n",pack.data.send_account,ctime(&ticks));
                    write(fp,server_buf,strlen(server_buf));
                    pack.data.ret_AAA = Exit_Account(&pack,&mysql);
                    pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    curfds --;
                    
                    if(epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&ev) < 0)
                        my_err("epoll ctl",__LINE__);

                    memset(server_buf,0,sizeof(server_buf));
                    sprintf(server_buf,"套接字编号为%d退出服务器\t\t\t\t\t%.24s\n",events[i].data.fd,ctime(&ticks));
                    write(fp,server_buf,strlen(server_buf));
                    close(events[i].data.fd);

                    continue;
                }

                else if(pack.type == SIGNAL_SEND)
                {
                    
                    
                    pack.data.ret_AAA = Signal_Chat(&pack);
                    //printf("%d : \n",ret_SC);
                    pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    if(send(events[i].data.fd,&pack,sizeof(pack),0) < 0)
                    {
                        printf("%d : error \n",__LINE__);
                    }

                    continue;
                }
                else if(pack.type == GROUP_CHAT)
                {
                   
                    pack.data.ret_AAA = Group_Chat(&pack);
                    pack.data.recv_fd = 55;
                    
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if (pack.type == ADD_FRIEND)
                {
                    
                    
                    pack.data.ret_AAA = Add_Friends(&pack,&mysql);
                //处理返回值
                    pack.data.recv_fd = 55;
                    if(send(events[i].data.fd,&pack,sizeof(pack),0) < 0)
                    {
                        printf("%d : send_error",__LINE__);
                    }
 
                    continue;
                }
                else if(pack.type == MANAGE_FRIEND)
                {
                    if(pack.data.recv_fd == 1)
                    {
                        pack.data.ret_AAA = Manage_Friend(pack.data.recv_account,pack.data.send_account);
                       
                        pack.data.recv_fd = 55;
                        pack.data.send_fd = 0;
                        send(events[i].data.fd,&pack,sizeof(pack),0);
                    }
                    continue;

                }
                else if(pack.type == MANAGE_GROUP)
                {
                    if(pack.data.recv_fd == 1)
                    {
                        pack.data.ret_AAA = Manage_Group(&pack);
                        pack.data.recv_fd = 55;
                        pack.data.send_fd = 0;
                        send(events[i].data.fd,&pack,sizeof(pack),0);
                        
                    }
                    continue;
                }
                else if(pack.type == ADD_GROUP)
                {
                    
                    pack.data.ret_AAA = Add_Group(pack.data.recv_account,pack.data.send_account);
                    pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == DEL_FRIEND)
                {
                   
                    pack.data.ret_AAA = Del_Friend(&pack,&mysql);
                     pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == LOOK_FRIEND)
                {
                    pack.data.recv_fd = 66;
                    pack.data.send_fd = 0; 
                    send(events[i].data.fd,&pack,sizeof(pack),0);                   
                    Look_Friend(events[i].data.fd,pack.data.send_account);
                    continue;
                }
                else if (pack.type == LOOK_CHAT_RECORD)
                {
                   
                    Look_Chat_Record(events[i].data.fd,pack.data.send_account);
                    
                    continue;
                }
                else if(pack.type == OUT_MEMBER)
                {
                    
                    pack.data.ret_AAA = Out_Member(pack.data.send_account,pack.data.recv_account,pack.data.send_fd);
                    pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    send(events[i].data.fd ,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == SET_REALTION)
                {
                    pack.data.recv_fd = 55;
                    pack.data.ret_AAA = Set_Realtion(pack.data.send_account,pack.data.recv_account,pack.data.send_fd);
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;

                }
                else if(pack.type == SET_QUN)
                {
                    pack.data.ret_AAA = Set_Qun(&pack);
                    pack.data.send_fd = 0;
                    pack.data.recv_fd = 55;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == CREATE_GROUP)
                {
                    pack.data.recv_fd = 55;
                    pack.data.ret_AAA = Registed_Group(&pack);
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == FREE_GROUP)
                {
                    pack.data.recv_fd = 55;
                    pack.data.ret_AAA = Free_Group(pack.data.recv_account,pack.data.send_account);
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == LOOK_GROUP_MEMBER)
                {
                    pack.data.ret_AAA = Look_Group_Member(events[i].data.fd,pack.data.recv_account,pack.data.send_account);
                    pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    
                    continue;
                }
                else if(pack.type == LOOK_GROUP)
                {
                    Look_Group(events[i].data.fd,pack.data.send_account);

                    continue;
                }
                else if(pack.type == RETURN_GROUP)
                {
                    pack.data.recv_fd = 55;
                    pack.data.send_fd = 0;
                    pack.data.ret_AAA = Return_Group(pack.data.recv_account,pack.data.send_account);
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
                else if(pack.type == SEND_FILE)
                {
                    
                    
    
                    pack.data.ret_AAA = Recv_File(events[i].data.fd,&pack);
                    pack.data.send_fd = 0;
                    pack.data.recv_fd = 55;
                    printf("pack.data.ret_AAA = %d\n",pack.data.ret_AAA);
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;

                }
                else if(pack.type == RECV_FILE)
                {

                    pack.data.ret_AAA = Send_File(events[i].data.fd,&pack);
                    
                    continue;
                }
                else if(pack.type == MANAGE_FILE)
                {
                    Manage_File(pack.data.send_account,pack.data.recv_account,pack.data.send_user);
                    continue;
                }
                else if(pack.type == MANAGE_MESSAGE)
                {
                    Manage_Message(pack.data.send_account,pack.data.recv_account,pack.data.write_buf);
                    continue;
                }
                else if(pack.type == LOOK_MESSAGEBOX)
                {
                    Look_MessageBox(events[i].data.fd,pack.data.send_account);
                    
                    continue;
                }
              /*
                else if (pack.type == MANAGE_FRIEND)
                {
                    
                    if(pack.data.recv_fd == 1)
                    {
                        char string[1024] = "\0";
                        sprintf(string,"insert into friends values(%d,%d,%d)",pack.data.recv_account,pack.data.send_account,1);
                        mysql_query(&mysql,string);
                        memset(string,0,sizeof(string));
                        sprintf(string,"insert into friends values(%d,%d,%d)",pack.data.send_account,pack.data.recv_account,1);
                        mysql_query(&mysql,string);
                        ret_MF = 1;
                        send(events[i].data.fd,&ret_MF,sizeof(ret_MF),0);
                    }
                    else 
                    {
                        
                        send(events[i].data.fd,&ret_MF,sizeof(ret_MF),0);
                        
                    }
                }
                */

/*
                else if(pack.type == 1)   // 登录
                {
                    /*登陆 通过DATA中的send_account 在user_data表中查找密码
                        密码暂且存放在send_buf 中，strcmp比较
                        返回一个int型数据（0 失败，重新输入）
                                       （1 成功）
                               
                }
                else if (pack.type == 2)   //注册
                {
                    /* 通过传入的type直接判断，
                    把账户存放在send_account 中，密码存放在send_user中，
                    密保问题放在read_buf中，密保答案放在write_buf中
                    操作数据库添加至user_data表中
                    
                }
                else if(pack.type == 3)   //修改密码
                {
                    /*
                        将账户存在send_account 中，密保答案放在send_user中，修改后的密码放在write_buf中
                        操作数据库
                        strcmp   若密保答案正确，
                        update set user_data password = '   ' where account
                        修改成功返回1.，失败返回0 
                    
                }
                else if(pack.type == 4)  //添加好友
                {
                    /*
                        操作数据库 user_data表 查找 发送好友请求的客户端昵称
                        将请求发送到对应的消息盒子表
                        等待接受客户端处理，
                        strcmp比较是否同意，
                        （同意，拒绝，忽略，忽略处理类似与拒绝处理，只是不给发送请求的客户端回应）
                    
                }
                else if (pack.type == 5)   //查看好友列表
                {
                    /*
                        select friend_user from friends where user = '   ';
                        获取到的friend_user作为下一次mysql的语句
                        select (account,nickname,users_state) from user_data where account == 'friend_user';
                        获取到在线 客户端彩色打印
                    
                }
                else if(pack.type == 6)   // 查看聊天记录
                {
                    /*
                        select (send_user,messages) from chat_messages where recv_user = ' *****'
                    
                } 
                else if(pack.type == 7)   //删除好友
                {
                    /*
                        从客户端接收到要删除的好友账户,保存到DATA.send_account和该账户保存到DATA.recv.account
                        delete from friends where user = '当前账户' friend_user = '要删除的好友账户'
                        修改消息表 该账户无法查看删除好友的信息
                        update set chat_messages recv_can_look =0 where recv_user = '该账户'
                    
                }
                else if (pack.type == 8)   //设置好友状态，(特别关心，黑名单，取消特别关心)
                {
                    /*
                        从客户端接收到要设置的账户存到DATA.send_account
                        设置friends表的reation值   （0.普通 1.特别关心，-1.取消特别关心，2.移出黑名单 -2.加入黑名单）
                    
                }
                else if (pack.type == 9)   //创建群         1.群主  2.管理员  0.群成员
                {
                    /*
                        从客户端获取群名，设置该账户群地位为群主（1）
                        得到一个群号存在group_account 
                        加入该账户的账户和昵称 以及群地位
                        insert
                    
                }
                else if(pack.type == 10)  //解散群
                {
                    /*
                        在group表中判断该账户的群地位
                        if(group_state == 1)
                            delete from group_member where group_account ='   ';
                        else 
                            send()  错误提示给客户端
                    
                }
                else if(pack.type == 11)   // 群聊
                {
                    /*
                        通过群号获取到好友账户
                        给该账户的消息盒子发送数据
                    
                }
                else if(pack.type == 12)  //查看群成员
                {
                    /*
                        select (group_member_account  , group_member_nickname  , group_state) from group_member where group_account ='  ';
                    
                }
                else if(pack.type == 13)     //查看以加群
                {
                    /*
                        通过账户查找群号以及群名
                        select (group_account ,group_name) from group_member where group_member_account ='该账户'
                    *
                }
                else if(pack.type == 14)   //设置群管理员
                {
                    /*
                        查看该账户是否为群主
                        if（是）
                            则可以修改group_member 表中 group_state 的信息
                            update
                        else
                            send
                            返回错误提示信息
                    
                }
                else if(pack.type == 15)   // 踢人
                {
                    /*
                        查看该账户是否为群主
                        if（是）
                            通过群成员账户删除信息
                        else
                            返回错误提示信息
                            send
                    *
                }
                else if(pack.type == 16)   //私聊
                {
                    /*
                        通过账户把消息发到消息盒子，等待对方接受
                    *
                }
                
 */               
                
                

            }
        }
    }

    char string[1024] = "\0";
    sprintf(string,"update user_data set user_socket = 0,user_state = 0");
    mysql_query(&mysql,string);
    mysql_close(&mysql);
    mysql_library_end();
    close(fp);
    close(epfd);

}


void signal_fun(int signo)
{
  
    char string[1024] = "\0";
    sprintf(string,"update user_data set user_socket = 0,user_state = 0");
    mysql_query(&mysql,string);
    mysql_close(&mysql);
    mysql_library_end();
    close(fp);
    close(epfd);
    exit(-1);
}


//-1 账户不存在，1 登录成功，0 密码错误
int Login_Account(PACK *pack,MYSQL *mysql)
{
    mysql_node *current;
    current = head_user;
    int flag = 0;
    //printf("%d %saaaaaaaaaa\n",pack->data.send_account,pack->data.send_user);
    while(current != NULL)
    {
        if(current->item.user.account==pack->data.send_account)
        {
            flag = 1;
           // printf("AAA%sAAAaa",current->item.user.password);
            if(strcmp(current->item.user.password,pack->data.send_user) == 0)
            {
                flag = 2;
                current->item.user.user_socket = CONN_FD;
                current->item.user.user_state = 1;
            }
            break;
            
            
            
            
        }
        current = current -> next;
    }
    printf("%d\n",flag);
    if(flag == 2)
    {
        char string[1024] = "\0";
        ACCOUNT = pack->data.send_account;
       
        sprintf(string,"update user_data set user_state = 1,user_socket = %d where account = %d",CONN_FD,ACCOUNT);
        if((mysql_query(mysql,string)) == 0)
        {
            return 1;
        }
        
    }
    else if(flag ==1)
        return 0;
    else if(flag == 0)
        return -1;

}

int Registed_Account(PACK *pack,MYSQL *mysql)
{
    int i;
    int account = 0;
    int a[100] = {0};
    srand(time(NULL));
    for(i = 0;i < 100;i++)
    {
        a[i] = rand()%100000000 + 10000000;

    }
    i = rand()%100;
    account = a[i];
    mysql_node * new_user,*current;
    new_user = (mysql_node *)malloc(sizeof(mysql_node));
    new_user->item.user.account = account;
    strcpy(new_user->item.user.nickname,pack->data.recv_user);
    strcpy(new_user->item.user.password,pack->data.send_user);
    strcpy(new_user->item.user.mibao,pack->data.read_buf);
    strcpy(new_user->item.user.daan,pack->data.write_buf);
    new_user->next = NULL;
    current = head_user;
    while(current != NULL)
        current = current -> next;

    mysql_list(&head_user,new_user);
    char string[4096] = "\0";
    sprintf(string,"insert into user_data values(%d,'%s','%s',0,0,'%s','%s')",account,pack->data.recv_user,pack->data.send_user,pack->data.read_buf,pack->data.write_buf);
    
    if((mysql_query(mysql,string)) == 0)
    {
        memset(server_buf,0,sizeof(server_buf));
        sprintf(server_buf,"申请新账户\n 账户：%d 账户名：%s 密码：%s  密保：%s 答案: %s\n"
        ,account,pack->data.recv_user,pack->data.send_user,pack->data.read_buf,pack->data.write_buf);
        write(fp,server_buf,strlen(server_buf));
        
        return account;

    }

    else 
        return -1;

}

int Find_Password(PACK *pack,MYSQL *mysql)
{
    mysql_node * current;
    current = head_user;
    int flag = 0;
    while(current != NULL)
    {
        if(current->item.user.account == pack->data.send_account)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        strcpy(pack->data.write_buf,current->item.user.mibao);
        strcpy(pack->data.read_buf,current->item.user.daan);
        strcpy(pack->data.send_user,current->item.user.password);
        pack->data.send_fd = 123456;
        return 1;
    }
    else 

        return 1;

    /*
    char string[1024] = "\0";
    sprintf(string,"select mibao,daan,password from user_data where account = %d",pack->data.send_account);
    if(mysql_query(mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(mysql);
        MYSQL_ROW row;
       while((row = mysql_fetch_row(result)) != NULL)
        {

            
            strcpy(pack->data.write_buf,row[0]);
            strcpy(pack->data.read_buf,row[1]);

            strcpy(pack->data.send_user,row[2]);

        }
        
        return 1;
  
    }

    return -1;
    */
}

int Change_Password(PACK *pack,MYSQL * mysql)
{
    mysql_node *current;
    current = head_user;
    int flag = 0;
    while(current != NULL)
    {
        if(current -> item.user.account == pack->data.send_account)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }
    if(flag == 1)
    {
        strcpy(current->item.user.password,pack->data.send_user);
        char string[1024] = "\0";
        sprintf(string,"update user_data set password = '%s' where account = %d",pack->data.send_user,pack->data.send_account);
        mysql_query(mysql,string);
        memset(server_buf,0,sizeof(server_buf));
        time_t ticks;
        ticks = time(NULL);
        sprintf(server_buf,"账户： %d 修改密码为 %s-----------------------%24s\n",pack->data.send_account ,pack->data.send_user,ctime(&ticks));
        write(fp,server_buf,strlen(server_buf));    
        return 1;
    }
    else 
        return -1;
    
    
   
}

int Exit_Account(PACK *pack,MYSQL *mysql)
{
    mysql_node * current;
    current = head_user;
    while(current != NULL)
    {
        if(current->item.user.account == pack->data.send_account)
        {
            current->item.user.user_state = 0;
            current->item.user.user_socket = 0;
            break;
        }
        current = current->next;
    }

    char string[1024] = "\0";
    sprintf(string,"update user_data set user_state = 0,user_socket = 0 where account = %d",pack->data.send_account);
    printf("%s\n",string);
    if(mysql_query(mysql,string) == 0)
    {

        return 1;
    }
    else 
        return -1;
}


int getNowTime(char *nowTime)
{
    char acYear[5] = {0};
    char acMonth[5] = {0};
    char acDay[5] = {0};
    char acHour[5] = {0};
    char acMin[5] = {0};
    time_t now;
    struct tm* timenow;
    time(&now);
    timenow = localtime(&now);
    strftime(acYear,sizeof(acYear),"%Y",timenow);
    strftime(acMonth,sizeof(acMonth),"%m",timenow);
    strftime(acDay,sizeof(acDay),"%d",timenow);
    strftime(acHour,sizeof(acHour),"%H",timenow);
    strftime(acMin,sizeof(acMin),"%M",timenow);
    strncat(nowTime, acYear, 4);
    strncat(nowTime, acMonth, 2);
    strncat(nowTime, acDay, 2);
    strncat(nowTime, acHour, 2);
    strncat(nowTime, acMin, 2);
    return 0;
}


//return 1  没有该好友，可添加
//return 0  有该好友，不能重复添加
//return 2 账户不存在
int Add_Friends(PACK *pack,MYSQL *mysql)
{
    int flag = 0;
    int online_flag = 0;
    int fd = 0;
    mysql_node * current;
    current = head_user;
    printf("%d xiang %d fasong haoyouqingqiu \n",pack->data.send_account,pack->data.recv_account);
    while(current!= NULL)
    {
        if(current->item.user.account == pack->data.recv_account)
        {
            //要添加的账户存在
            flag = 1;
            break;
        }
        current = current -> next;
    }
    current = head_friends;
    while(current != NULL)
    {
        if(current->item.friends.user == pack->data.send_account && current->item.friends.friend_user == pack->data.recv_account)
        {
            //该好友存在
            flag = 2;
            break;
        }
        current = current -> next;
    }
    current = head_user;
    while(current != NULL)
    {
        printf("%d\t%d\n",current->item.user.account,current->item.user.user_socket);
        if(current->item.user.account == pack->data.recv_account)
        {
            
            if(current->item.user.user_state == 1)
            {
                online_flag = 1;
                printf("%d\n%d\n",current->item.user.account,current->item.user.user_socket);
                fd = current->item.user.user_socket;
                break;
            }
            break;
        }
        current = current->next;

    }

    if(flag == 2)
    {
        return 0;
    }
    else if(flag == 1)
    {
        printf("...........\n");
        mysql_node * new_requst;
        new_requst = (mysql_node *)malloc(sizeof(mysql_node));
        new_requst->item.chat.send_user = pack->data.send_account;
        new_requst->item.chat.recv_user = pack->data.recv_account;
        strcpy(new_requst->item.chat.messages,"好友请求...");
        new_requst->item.chat.send_can_look = 1;
        new_requst->item.chat.recv_can_look = 1; 
        new_requst->item.chat.message_type = 3;
        char nowTime[32] = {0};
        getNowTime(nowTime);
        strcpy(new_requst->item.chat.date,nowTime);
        mysql_list(&head_chat,new_requst);
        new_requst = head_friends;
        while(new_requst != NULL)
        {
            printf("%d %d %d\n",new_requst ->item.friends.user,new_requst->item.friends.friend_user,new_requst->item.friends.realtion);
            new_requst = new_requst->next;

        }
        char string[1024] = "\0";
        memset(string,0,sizeof(string));         
        sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",
        pack->data.send_account,pack->data.recv_account,"好友请求",1,1,3,nowTime);
        mysql_query(mysql,string);
        if(online_flag == 1)
        {
            printf(".........\n");
            pack->data.send_fd = 3;
            send(fd,pack,sizeof(*pack),0);
            return 1;
        }
        return 1;
    }
    
    else if(flag == 0)
    {
        return 2;
    }

    /*
    int ret_FA = Find_Account(pack->data.recv_account);

    printf("ret_FA%d\n",ret_FA);
    if(ret_FA == -1)
        return -1;   
    int ret_FFR = Find_Friend_Realtion(pack);
    printf("ret_FFR : %d\n",ret_FFR);
    if(ret_FFR == 3 && ret_FA == 1)
    {
        
        //保存到chat_messages,
        char nowTime[32] = {0};
        getNowTime(nowTime);
        char string[1024] = "\0";
        sprintf(string,"select user_state,user_socket from user_data where account = %d",pack->data.recv_account);
        mysql_query(mysql,string);
        memset(server_buf,0,sizeof(server_buf));
        sprintf(server_buf,"账户 : %d 向 账户 : %d 发送好友请求\t\t\t%24s\n",pack->data.send_account,pack->data.recv_account,nowTime);
        write(fp,server_buf,strlen(server_buf));
            MYSQL_RES * result = mysql_store_result(mysql);
            MYSQL_ROW row = mysql_fetch_row(result);
            memset(string,0,sizeof(string));
                sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",
                pack->data.send_account,pack->data.recv_account,"好友请求",1,1,3,nowTime);
                if(mysql_query(mysql,string) < 0)
                    printf("query error!\n");
           
                if(atoi(row[0]) == 1)
                {
                    printf("........");
                    send(atoi(row[1]),pack,sizeof(*pack),0);
                    pack->data.send_fd = 0;

                    return 1;

                }
                pack->data.send_fd = 0;
                return 1;
    }
    else if(ret_FFR == 1 || ret_FFR == 2)
        return 0;
    else 
        return -1;
        */


}

//设计思路：
//0 黑名单
//1 发送完成
//-1 不是好友关系
//不是好友关系 返回-1  是好友发送消息 返回0
int Signal_Chat(PACK *pack)
{
    
    int flag = 0;
    mysql_node * current;
    current = head_friends;
    while(current != NULL)
    {
        if(current->item.friends.user == pack->data.send_account && current->item.friends.friend_user == pack->data.recv_account )
        {
            flag = 1;
            if(current->item.friends.realtion == 1 || current->item.friends.realtion == 2)
                flag = 2;
            break;
        }
        current = current->next;
    }
    
    int realtion = 0;
    //把该好友加入黑名单不能发送消息
    if(flag == 1)
        return 0;
    else if(flag == 0)
        return -1;
    else if(flag == 2)
    {
        printf("进入\n");
        mysql_node * current_friend;
        current_friend = head_friends;
        while(current_friend != NULL)
        {
            printf("aaaaa\n");
            if(current_friend -> item.friends.user == pack->data.recv_account && current_friend -> item.friends.friend_user == pack->data.send_account)
            {
                realtion = current_friend -> item.friends.realtion;
                break;
            }
            current_friend = current_friend->next;
        }
        
        if(realtion == 1 || realtion ==2 )
        {
            mysql_node * new_messages,*current;
            new_messages = (mysql_node *)malloc(sizeof(mysql_node));
            new_messages -> item.chat.send_user = pack->data.send_account;
            new_messages -> item.chat.recv_user = pack->data.recv_account;
            strcpy(new_messages -> item.chat.messages,pack->data.write_buf);
            new_messages -> item.chat.send_can_look = realtion;
            new_messages -> item.chat.recv_can_look = 1;
            new_messages -> item.chat.message_type = 1;
            char nowTime[32] = {0};
            getNowTime(nowTime);
            strcpy(new_messages -> item.chat.date,nowTime);
            new_messages->next = NULL;
            current = head_chat;
            
            
            while(current != NULL)
                current = current -> next;

            mysql_list(&head_chat,new_messages);
            current = head_chat;
            while(current != NULL)
            {
               
                current = current -> next;
            }
            current = head_user;
            int fd;
            while(current != NULL)
            {
                if(current ->item.user.account == pack->data.recv_account)
                {
                    fd = current -> item.user.user_socket;
                }
                current = current->next;
            }
            
            if(fd != 0)
            {
                send(fd,pack,sizeof(*pack),0);
            }
            char string[4096] = "\0";
            sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')"   
                ,pack->data.send_account,pack->data.recv_account,pack->data.write_buf,realtion,1,1,nowTime);
            
            if(mysql_query(&mysql,string) == 0)
            {
                return 1;
            }
            
        }
    }

     /*   int ret_FFR = -1; 
        //pack.data.send_fd         1.xiaoxi     2.wenjian      3.haoyouqingqiu
        char string1[1024] = "\0";
        sprintf(string1,"select realtion from friends where user = %d and friend_user = %d",pack->data.send_account,pack->data.recv_account);
        //查找接收者消息对发送者消息的关系
        printf("%s\n",string1);
        mysql_query(&mysql,string1);
        MYSQL_RES * result1 = mysql_store_result(&mysql);
        MYSQL_ROW row1 = mysql_fetch_row(result1);
        if(row1 == NULL)
            return -1;
        if(atoi(*row1) == 0)
            return 0;
        
        
        //保存到chat_messages,
        
        memset(string1,0,sizeof(string1));
        sprintf(string1,"select realtion from friends where user = %d and friend_user = %d",pack->data.recv_account,pack->data.send_account);
        mysql_query(&mysql,string1);
        result1 = mysql_store_result(&mysql);
        row1 = mysql_fetch_row(result1);
        if(row1 == NULL)
            return 1;
        if(atoi(*row1) == 0)
            return 1;
        
        ret_FFR = atoi(*row1);
        /*
        if(ret_FFR == 3 || ret_FFR == 0)
        {
            return -1;
        }
        
        if(ret_FFR == 1 || ret_FFR == 2)
        {
            char nowTime[32] = {0};
            getNowTime(nowTime);
            char string[4096] = "\0";
            
            // ret_FFR  haoyouguanxi     pack->data.send_fd xiaoxileixing   1.weichakan
            pack->data.recv_fd = ret_FFR;
            sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')"   
                ,pack->data.send_account,pack->data.recv_account,pack->data.write_buf,1,ret_FFR,pack->data.send_fd,nowTime);
            
            
            time_t ticks;
            ticks = time(NULL);
            if(mysql_query(&mysql,string) == 0)
            {
                memset(server_buf,0,sizeof(server_buf));
                if(pack->data.send_fd == 1)
                    sprintf(server_buf,"%.24s...%d发送给%d消息：%s\n",ctime(&ticks),pack->data.send_account,pack->data.recv_account,pack->data.write_buf);
                else if(pack->data.send_fd == 3)
                    sprintf(server_buf,"%.24s...%d发送给%d好友请求\n",ctime(&ticks),pack->data.send_account,pack->data.recv_account);
                else if(pack->data.send_fd == 2)
                    sprintf(server_buf,"%.24s...%d发送给%d文件\n",ctime(&ticks),pack->data.send_account,pack->data.recv_account);
                write(fp,server_buf,strlen(server_buf));
            }
        
        //判断好友是否在线     如果在线，直接发送，修改chat_messages表中消息位为已发送
        //查看好友关系
        
            
            sprintf(string,"select user_state,user_socket from user_data where account = %d",pack->data.recv_account);
            mysql_query(&mysql,string);
            MYSQL_RES * result = mysql_store_result(&mysql);
            MYSQL_ROW row = mysql_fetch_row(result);
            
            
            if(atoi(row[0]) != 0 && pack->data.send_fd == 1)
            {

//尤其注意该处传参 
                pack->data.recv_fd = ret_FFR;             
                 
                send(atoi(row[1]),pack,sizeof(*pack),0);
                return 1;
            }
            

        }
        else 
        {
            return 1;
        }
        if(pack->data.send_fd == 2)
        {
            
        }
        return 1;
        
    */

}

//查看是否是好友关系，如果是返回关系标志位 ，不是返回 3 
//不是好友  3    黑名单 0 好友 1 特别关心2
int Find_Friend_Realtion(PACK *pack)
{
    if((Find_Account(pack->data.recv_account)) == -1)
        return 0;

    char string[1024] = "\0";
    sprintf(string,"select realtion from friends where user = %d and friend_user = %d",pack->data.send_account,pack->data.recv_account);
    printf("%d : %s\n",__LINE__,string);
    if(mysql_query(&mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW  row = mysql_fetch_row(result);
        if(row == NULL)
            return 3;    //不是好友关系
        else 
        {
            int i = atoi(row[0]);
            return i;
        }
    }
    else 
        return -1;
}


//查看该账户是否存在，如果存在返回1,不存在返回-1
int Find_Account(int account)
{
    char string[1024] = "\0";
    strcpy(string,"select account from user_data\0");
    printf("%d : %s\n",__LINE__,string);
    if(mysql_query(&mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row;
        while((row = mysql_fetch_row(result)) != NULL)
        {
            if(atoi(*row) == account)
            {
                //printf("cunzai\n");
                return 1;
            }
        }
        return -1;
    }
    else 
        return -1;
}


//ret 0 删除成功
//ret 1 删除失败
int Del_Friend(PACK * pack,MYSQL *mysql)
{
    mysql_node * current,*prev;
    current = head_user;
    int user_flag = 0;
    while(current != NULL)
    {
        if(current->item.user.account == pack->data.recv_account)
        {
            user_flag = 1;
            break;
        }
        current = current->next;
    }
    current = head_friends;
    int flag = 0;
    while(current != NULL)
    {
        if(current->item.friends.friend_user == pack->data.recv_account && current->item.friends.user == pack->data.send_account)
        {
            flag = 1;
            break;
        }
        current = current->next;
    }

    if(user_flag == 0)
    {
        return -1;
    }
    if(flag == 1 && user_flag == 1)
    {
        //printf("youhaoyou\n");
        current = head_friends;
        prev = head_friends;
        while(current != NULL)
        {
            if(current->item.friends.friend_user == pack->data.recv_account && current->item.friends.user == pack->data.send_account)
            {
                break;
            }
            prev = current;
            current = current->next;
        }
        if(current->next == NULL)
            prev->next = NULL;
        //printf("............\n");
        prev->next = current->next;
        //printf("............\n");
        //free(current);
        char string[1024] = "\0";
        //printf("dengdaishanchu\n");
        while(current != NULL)
        {
            printf("%d %d %d\n",current->item.friends.user,current->item.friends.friend_user,current->item.friends.realtion);
            current = current->next;
        }
        sprintf(string,"delete from friends where friend_user = %d and user = %d",pack->data.recv_account,pack->data.send_account);
        mysql_query(mysql,string);
        return 0;

    }
    else if(flag == 0)
    {
        return 1;
    }
    /*
    int ret_FFR = Find_Friend_Realtion(pack);
    if(ret_FFR == 3)
        return -1;
    if(ret_FFR == 0)
        return 0;
    else 
    {
        char string[1024] = "\0";
        sprintf(string,"delete from friends where friend_user = %d and user = %d",pack->data.recv_account,pack->data.send_account);
        if(mysql_query(mysql,string) == 0) 
        {
            memset(server_buf,0,sizeof(server_buf));
            time_t ticks;
            ticks = time(NULL);
            sprintf(server_buf,"账户 ： %d 删除好友 %d \t\t\t%24s\n",pack->data.send_account,pack->data.recv_account,ctime(&ticks));
            write(fp,server_buf,strlen(server_buf));      
            return 1;
        
        }
        else 
            return -2;
    }
    */
}

int Offline_Messages(int fd,int account)
{
    mysql_node * current;
    current = head_chat;
    int i = 0;
    while(current != NULL)
    {
        if(current->item.chat.recv_user == account)
        {
            if(current -> item.chat.send_can_look == 1 || current -> item.chat.send_can_look == 2)
            {
                i++;
            }
        }
        current = current->next;
    }
    /*
    int i = 0;
    char string[1024] = "\0";
    sprintf(string,"select message_type from chat_messages where  recv_user = %d and (send_can_look = 1 or send_can_look = 2)",account);
    if(mysql_query(&mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = NULL;
        
        while((row = mysql_fetch_row(result)) != NULL)
        {
            i++;
        }
    }
    */
    send(fd,&i,sizeof(i),0);
    return 0;
}

int Look_MessageBox(int fd,int account)
{

    printf("..........\n");
    BOX box;
    memset(&box,0,sizeof(box));

    mysql_node * current;
    current = head_chat;
    if(current == NULL)
    {
        printf("kong\n");
        return 0;
    }
    while(current != NULL)
    {
        printf("A");
        if(current->item.chat.recv_user == account && (current->item.chat.send_can_look == 1 || current->item.chat.send_can_look == 2))
        {
            box.number ++;
            box.type[box.number] = current->item.chat.message_type;
            if(box.type[box.number] == 1)       //xiaoxi
            {
                printf("xiaoxi");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 2)      //wenjian
            {
                printf("wenjian0");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.talk_number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 3)      //haoyouqingqiu
            {
                printf("haoyou");
                box.friend_number ++;
                box.plz_account[box.friend_number] = current->item.chat.send_user;
                strcpy(box.plz_date[box.friend_number],current->item.chat.date);
                printf("%d  : %s\n",box.plz_account[box.friend_number],box.plz_date[box.friend_number]);
            }
            else if(box.type[box.number] == 4)   //qunliao
            {
                printf("qunliao");
                box.plz_number ++;
                box.plz_group[box.plz_number] = current->item.chat.send_user;
                strcpy(box.write_buf[box.plz_number],current->item.chat.messages);
                strcpy(box.group_date[box.plz_number],current->item.chat.date);
                
            }
        }
        current = current -> next;
    }
    PACK pack;
    pack.data.recv_fd = 66;
    send(fd,&pack,sizeof(pack),0);
    sleep(1);
    send(fd,&box,sizeof(box),0);
    /*
    char string[1024] = "\0";
    sprintf(string,"select send_user,messages,message_type,date from chat_messages where  recv_user = %d and (send_can_look = 1 or send_can_look = 2)",account);
    printf("%s\n",string);
    if(mysql_query(&mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = NULL;
        while((row = mysql_fetch_row(result)) != NULL)
        {
            box.number ++;
            box.type[box.number] = atoi(row[2]);
        
            if(box.type[box.number] == 1)                    //消息
            {
                box.talk_number ++;
                box.message_account[box.talk_number] = atoi(row[0]);
                strcpy(box.message_date[box.talk_number],row[3]);
                strcpy(box.messages[box.talk_number],row[1]);
                printf("%d : %s \n",box.talk_number,box.messages[box.talk_number]);
            }
            else if(box.type[box.number] == 3)            //好友请求
            {
                box.friend_number ++;
                box.plz_account[box.friend_number] = atoi(row[0]);
                strcpy(box.plz_date[box.friend_number],row[3]);
            }
            else if(box.type[box.number] == 4)
            {
                box.plz_number ++;
                box.plz_group[box.plz_number] = atoi(row[0]);
                strcpy(box.write_buf[box.plz_number],row[1]);
                strcpy(box.group_date[box.plz_number],row[3]);
                printf("%d %s %s\n",box.plz_group[box.plz_number],box.write_buf[box.plz_number],box.group_date[box.plz_number]);
            }
            else if(box.type[box.number] == 2)
            {
//发送文件        
                box.talk_number ++;
                box.message_account[box.talk_number] = atoi(row[0]);
                strcpy(box.message_date[box.talk_number],row[3]);
                strcpy(box.messages[box.talk_number],row[1]);
            }

        }
    }
    
    PACK pack;
    pack.data.recv_fd = 66;
    send(fd,&pack,sizeof(pack),0);
    sleep(1);
    send(fd,&box,sizeof(box),0);
    */
}

int Manage_Friend(int recv_account,int send_account)
{
    mysql_node * current,*new_friend;
    current = head_friends;
    new_friend = (mysql_node *)malloc(sizeof(mysql_node));
    new_friend ->item.friends.user = recv_account;
    new_friend -> item.friends.friend_user = send_account;
    new_friend -> item.friends.realtion = 1;
    while(current != NULL)
        current = current -> next;
    mysql_list(&head_chat,new_friend);
    
    new_friend ->item.friends.user = send_account;
    new_friend -> item.friends.friend_user = recv_account;
    new_friend -> item.friends.realtion = 1;
    while(current != NULL)
        current = current -> next;
    mysql_list(&head_chat,new_friend);

    
    char string[1024] = "\0";
    sprintf(string,"insert into friends values(%d,%d,%d)",recv_account,send_account,1);
    if(mysql_query(&mysql,string) < 0)
    {
        printf("%d : query friends error!\n",__LINE__);
        return -1;
    }
    sprintf(string,"insert into friends values(%d,%d,%d)",send_account,recv_account,1);
    if(mysql_query(&mysql,string) < 0)
    {
        printf("%d : quert friends error! \n",__LINE__);
        return -1;
    }
    memset(string,0,sizeof(string));
    sprintf(string,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d ",send_account,recv_account);
    
    mysql_query(&mysql,string);

    memset(server_buf,0,sizeof(server_buf));
    time_t ticks;
    ticks = time(NULL);
    sprintf(server_buf,"账户 ： %d  和 账户 ： %d 已成为好友\t\t\t%24s\n",send_account,recv_account,ctime(&ticks));
    write(fp,server_buf,strlen(server_buf));
    PACK pack;
    current = head_chat;
    new_friend->item.chat.send_user = send_account;
    new_friend->item.chat.recv_user = recv_account;
    strcpy(new_friend -> item.chat.messages,"好友请求");
    new_friend->item.chat.send_can_look = 1;
    new_friend->item.chat.recv_can_look = 1;
    new_friend->item.chat.message_type = 3;

    char nowTime[32] = {0};
    getNowTime(nowTime);
    strcpy(new_friend -> item.chat.date,nowTime);
    new_friend->next = NULL;
    current = head_chat;
    while(current != NULL)
        current = current -> next;
    mysql_list(&head_chat,new_friend);
    pack.type = SIGNAL_SEND;
    pack.data.send_fd = 1;
    pack.data.recv_fd = 0;
    pack.data.ret_AAA = 0;
    pack.data.send_account = recv_account;
    pack.data.recv_account = send_account;
    strcpy(pack.data.write_buf,"已添加该用户为好友！");
    Signal_Chat(&pack);
    return 1;
}

int Look_Friend(int fd,int account)
{
    
    int friend;
    BOX box;
     memset(&box,0,sizeof(box));
   mysql_node * current,*current_user;
   current_user = head_user;
   current = head_friends;
   while(current != NULL)
   {
       if(current -> item.friends.user == account)
       {
           box.number ++;
           friend = current->item.friends.friend_user;   
           box.group_account[box.number] = current->item.friends.realtion;      // 好友关系
           box.plz_group[box.number] = friend;                                 //好友账户
           while(current_user != NULL)
           {
              
               if(current_user->item.user.account == friend)
               {
                   box.message_account[box.number] = current_user->item.user.user_state;    //好友状态
                   strcpy(box.group_date[box.number],current_user->item.user.nickname);    //好友昵称
                    break;
               }
               current_user = current_user->next;
           }
       }
       current = current->next;
   }
   current_user = head_user;
   
   while(current_user != NULL)
   {
       
        if(current_user -> item.user.account == account)
        {
            strcpy(box.group_date[0],current_user->item.user.nickname);
            break;
        }
        current_user = current_user->next;
   }
   box.plz_group[0] = account;
   printf("%d,%s\n", box.plz_group[0],box.group_date[0] );
   send(fd,&box,sizeof(box),MSG_WAITALL);
   //printf("查询好友\n");
   /*
    BOX box;
    memset(&box,0,sizeof(box));
    MYSQL_RES *result1;
    MYSQL_ROW row1;
    char string[1024] = "\0";
    char string1[1024] = "\0";
    sprintf(string,"select friend_user,realtion from friends where user = %d",account);
    mysql_query(&mysql,string);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = NULL;
    while((row = mysql_fetch_row(result)) != NULL)     
    {
        //row[0] 找到好友账户， row[1] 找到好友关系
        sprintf(string1,"select user_state,nickname from user_data where account = %d",atoi(row[0]));  
        mysql_query(&mysql,string1);
        result1 = mysql_store_result(&mysql);
        row1 = mysql_fetch_row(result1);
        //row1 保存好友状态
        box.number ++;
        box.plz_group[box.number] = atoi(row[0]);      //保存账户
        strcpy(box.group_date[box.number],row1[1]);    //保存昵称
        box.message_account[box.number] = atoi(row1[0]);           //保存好友是否在线
        //printf("%s--------- %d\n",box.group_date[box.number],box.message_account[box.number]);
        box.group_account[box.number] = atoi(row[1]);              //保存好友间关系
    }

    memset(string,0,sizeof(string));
    sprintf(string,"select nickname from user_data where account = %d",account);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    box.plz_group[0] = account;
    strcpy(box.group_date[0],*row);
    printf("box start\n");
    send(fd,&box,sizeof(box),MSG_WAITALL);
    printf("box over\n");
    //send(fd,0,sizeof(0),0);
    printf(" 0 over\n");
    */
}

int Manage_Message(int send_account,int recv_account,const char * string)
{
    mysql_node * current;
    current = head_chat;
    while(current != NULL)
    {
        if(current->item.chat.recv_user == recv_account && (strcmp(current->item.chat.messages,string) == 0) && current->item.chat.send_user == send_account)
        {
            current->item.chat.send_can_look = 0;
        }
        current = current->next;
    }

    char string1[1024] = "\0";
    sprintf(string1,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d and messages = '%s'",send_account,recv_account,string);
    if(mysql_query(&mysql,string1) != 0)
    {
        printf("%d : update error!\n",__LINE__);
    }

}

int Manage_File(int send_account,int recv_account,const char *string)
{
    char string1[1024] = "\0";
    sprintf(string1,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d and messages = '%s'"
    ,send_account,recv_account,string);
    mysql_query(&mysql,string1);
}
int Look_Chat_Record(int fd,int account)
{
    BOX box;
    memset(&box,0,sizeof(box));
    mysql_node * current;
    current = head_chat;
    while(current != NULL)
    {
        printf("A");
        if(current->item.chat.recv_user == account && (current->item.chat.send_can_look == 1 || current->item.chat.send_can_look == 2))
        {
            box.number ++;
            box.type[box.number] = current->item.chat.message_type;
            if(box.type[box.number] == 1)       //xiaoxi
            {
                printf("xiaoxi");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 2)      //wenjian
            {
                printf("wenjian0");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.talk_number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 3)      //haoyouqingqiu
            {
                printf("haoyou");
                box.friend_number ++;
                box.plz_account[box.friend_number] = current->item.chat.send_user;
                strcpy(box.plz_date[box.friend_number],current->item.chat.date);
                printf("%d  : %s\n",box.plz_account[box.friend_number],box.plz_date[box.friend_number]);
            }
            else if(box.type[box.number] == 4)   //qunliao
            {
                printf("qunliao");
                box.plz_number ++;
                box.plz_group[box.plz_number] = current->item.chat.send_user;
                strcpy(box.write_buf[box.plz_number],current->item.chat.messages);
                strcpy(box.group_date[box.plz_number],current->item.chat.date);
                
            }
        }
        current = current -> next;
    }
    PACK pack;
    pack.data.recv_fd = 66;
    send(fd,&pack,sizeof(pack),0);
    sleep(1);
    send(fd,&box,sizeof(box),0);
    /*
    char string[1024] = "\0";
    sprintf(string,"select send_user,messages,message_type,date from chat_messages where  recv_user = %d and recv_can_look = 1",account);
    if(mysql_query(&mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW row = NULL;
        while((row = mysql_fetch_row(result)) != NULL)
        {
            box.number ++;
            box.type[box.number] = atoi(row[2]);
        
            if(box.type[box.number] == 1)                    //消息
            {
                box.talk_number ++;
                box.message_account[box.talk_number] = atoi(row[0]);
                strcpy(box.message_date[box.talk_number],row[3]);
                strcpy(box.messages[box.talk_number],row[1]);
                printf("%d : %s \n",box.talk_number,box.messages[box.talk_number]);
            }
            else if(box.type[box.number] == 3)            //好友请求
            {
                box.friend_number ++;
                box.plz_account[box.friend_number] = atoi(row[0]);
                strcpy(box.plz_date[box.friend_number],row[3]);
            }
            else if(box.type[box.number] == 4)
            {
                box.plz_number ++;
                box.plz_group[box.plz_number] = atoi(row[0]);
                strcpy(box.group_date[box.plz_number],row[3]);
            }
            else if(box.type[box.number] == 2)
            {
//发送文件          
                box.file_number ++;
                box.file_account[box.file_number] = atoi(row[0]);
                strcpy(box.file_date[box.file_number],row[3]);
                strcpy(box.file[box.file_number],row[1]);
                
            }

        }
    }
    PACK pack;
    pack.data.recv_fd = 66;
    send(fd,&pack,sizeof(pack),0);
    sleep(1);
    send(fd,&box,sizeof(box),0);
    */
}     


 
//-1   不是好友   1 设置成功 
int Set_Realtion(int send_account,int recv_account,int state)
{
    int flag = 0;
    mysql_node *current;
    current = head_friends;
    while(current != NULL)
    {
        if(current -> item.friends.user == recv_account && current -> item.friends.friend_user == send_account)
        {
            flag = 1;
            current->item.friends.realtion = state;
            char string[1024] = "\0";
            sprintf(string,"update friends set realtion = %d where user = %d and friend_user = %d",state,send_account,recv_account);
            mysql_query(&mysql,string);
            return 1;
        }
        current = current -> next;
    }
    if(flag == 0)
        return -1;
/*
    if((Find_Account(recv_account)) == -1)
        return 0;

    char string[1024] = "\0";
    sprintf(string,"select realtion from friends where user = %d and friend_user = %d",send_account,recv_account);

    if(mysql_query(&mysql,string) == 0)
    {
        MYSQL_RES *result = mysql_store_result(&mysql);
        MYSQL_ROW  row = mysql_fetch_row(result);
        if(row == NULL)
            return -1;    //不是好友关系
        else 
        {
           memset(string,0,sizeof(string));
           sprintf(string,"update friends set realtion = %d where user = %d and friend_user = %d",state,send_account,recv_account);
           if(mysql_query(&mysql,string) == 0)
           {
               memset(server_buf,0,sizeof(server_buf));
               time_t ticks;
               ticks = time(NULL);
               sprintf(server_buf,"账户 : %d 设置账户%d  为 %d(0.黑名单 1.一般好友 2.特别关心)\t\t%24s\n",send_account,recv_account,state,ctime(&ticks));
               write(fp,server_buf,strlen(server_buf));
               return 1;
           }
           else 
            return 0;
        }
    }
    else 
        return 0;
        */
}

int Registed_Group(PACK *pack)
{
    mysql_node * current,*new_group;
    current = head_data;
    new_group = (mysql_node *)malloc(sizeof(mysql_node));
    int i;
    int group_account = 0;
    int a[100] = {0};
    srand(time(NULL));
    for(i = 0;i < 100;i++)
    {
        a[i] = rand() %10000000 + 1000000;

    }
    i = rand()% 100;
    group_account = a[i];
    new_group->item.data.group_account = group_account;
    new_group->item.data.group_number = pack->data.send_account;
    strcpy(new_group->item.data.group_name,pack->data.send_user);
    new_group->next = NULL;
    while(current != NULL)
    {
        current = current->next;
    }
    mysql_list(&head_data,new_group);
    char string[1024] = "\0";
    sprintf(string,"insert into group_data values(%d,'%s',%d)",group_account,pack->data.send_user,pack->data.send_account);
    mysql_query(&mysql,string);
    
    int flag = 0;
    int qunzhu;
    char nickname[81] = "\0";
    current = head_user;
    while(current != NULL)
    {
        if(current->item.user.account == pack->data.send_account)
        {
            strcpy(nickname,current->item.user.nickname);
            qunzhu = pack->data.send_account;
            flag = 1;
            break;
        }
        current = current -> next;
    }
    memset(new_group,0,sizeof(*new_group));
    if(flag == 1)
    {
        new_group->item.member.group_account = group_account;
        strcpy(new_group->item.member.group_name , pack->data.send_user);
        new_group->item.member.group_member_account = qunzhu;
        strcpy(new_group->item.member.group_member_nickname,nickname);
        new_group->item.member.group_state = 1;
        new_group->next = NULL;
        mysql_list(&head_member,new_group);
        char string[1024] = "\0";
        memset(string,0,sizeof(string));
        sprintf(string,"insert into group_member values(%d,'%s',%d,'%s',%d)",group_account,pack->data.send_user,pack->data.send_account,nickname,1);
        if(mysql_query(&mysql,string) == 0)
        {
            
            memset(server_buf,0,sizeof(server_buf));
            time_t ticks;
            ticks = time(NULL);
            sprintf(server_buf,"账户 ： %d 申请群 %d 成功\t\t%24s\n",pack->data.send_account,group_account,ctime(&ticks));
            write(fp,server_buf,strlen(server_buf));
            return group_account;
        }
        return -1;
    }
    return -1;
    
        
    /*
    memset(string,0,sizeof(string));
    sprintf(string,"select nickname from user_data where account = %d",pack->data.send_account );
    char nickname[50]= "\0";
    mysql_query(&mysql,string);
    MYSQL_RES * result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    strcpy(nickname,*row);
    memset(string,0,sizeof(string));
    sprintf(string,"insert into group_member values(%d,'%s',%d,'%s',%d)",group_account,pack->data.send_user,pack->data.send_account,nickname,1);
    if(mysql_query(&mysql,string) == 0)
    {
        
        memset(server_buf,0,sizeof(server_buf));
        time_t ticks;
        ticks = time(NULL);
        sprintf(server_buf,"账户 ： %d 申请群 %d 成功\t\t%24s\n",pack->data.send_account,group_account,ctime(&ticks));
        write(fp,server_buf,strlen(server_buf));
        return group_account;
    }
    return -1;
*/
}

//-1 不在群内
//0 无权解散群
//1 解散成功
int Free_Group(int account,int group)
{
    int flag = 0;
    mysql_node * current;
    current = head_member;
    while(current != NULL)
    {
        if(current->item.member.group_account = group && current->item.member.group_member_account == account)
        {
            flag = 1;
            if(current->item.member.group_state == 1)
            {
                flag = 2;
                break;
            }
        }
        current = current->next;
    }
    if(flag == 1)
        return 0;
    else if(flag == 0)
        return -1;

    else if(flag == 2)
    {
        current = head_member;
        mysql_node * prev = head_member;
        while(current != NULL)
        {
            if(current->item.member.group_account == group)
            {
                prev -> next = current->next;
            }
            prev = current;
            current = current -> next;
        }
        char string[1024] = "\0";
        sprintf(string,"delete from group_member where group_account = %d",group);
        
        if(mysql_query(&mysql,string) == 0)
        {
            memset(server_buf,0,sizeof(server_buf));
            time_t ticks;
            ticks = time(NULL);
            sprintf(server_buf,"群%d被解散\t\t\t\t%24s\n",group,ctime(&ticks));
            write(fp,server_buf,strlen(server_buf));
        }
        current = head_data;
        prev = head_data;
        while(current != NULL)
        {
            if(current->item.data.group_account == group)
                break;
            prev = current;
            current = current->next;
        }
        if(current->next == NULL)
            prev->next = NULL;
        prev->next = current->next;
        memset(string,0,sizeof(string));
        sprintf(string,"delete from group_data where group_number = %d and group_account = %d",account,group);
        mysql_query(&mysql,string);
        return 1;
    }
    /*
    char string[1024] = "\0";
    sprintf(string,"select group_account from group_data where group_number = %d",account);
    mysql_query(&mysql,string);
    
    MYSQL_RES * result = mysql_store_result(&mysql);
    MYSQL_ROW row;
    int ret = 0; 
    while((row = mysql_fetch_row(result)) != NULL)
    {
        if(group == atoi(*row))
        {
            ret = 1;
            break;
        }
    }
    if(ret == 0)
        return -1;
    memset(string,0,sizeof(string));
    sprintf(string,"delete from group_data where group_number = %d and group_account = %d",account,group);
    
    if(mysql_query(&mysql,string) < 0)
    {
        printf("%d : delete error! ",__LINE__);
        return -1;
    }
    memset(string,0,sizeof(string));
    sprintf(string,"delete from group_member where group_account = %d",group);
    
    if(mysql_query(&mysql,string) == 0)
    {
        memset(server_buf,0,sizeof(server_buf));
        time_t ticks;
        ticks = time(NULL);
        sprintf(server_buf,"群%d被解散\t\t\t\t%24s\n",group,ctime(&ticks));
        write(fp,server_buf,strlen(server_buf));
    }

    return 1;
    */

}

//-1 无权查看   1 可查看
int Look_Group_Member(int fd,int account,int group)
{

    BOX box;
    memset(&box,0,sizeof(box));
    int flag = 0;
    mysql_node * current;
    current = head_member;
    while(current != NULL)
    {
        if(current -> item.member.group_account == group && current -> item.member.group_member_account == account)
        {
            flag = 1 ;
            break;
        }
        current = current->next;
    }
    if(flag == 0)
        return -1;
    if(flag == 1)
    {
        current = head_member;
        while(current != NULL)
        {
            if(current -> item.member.group_account == group)
            {
                box.number ++;
                box.message_account[box.number] = current->item.member.group_member_account;
                box.type[box.number] = current->item.member.group_state;
                strcpy(box.message_date[box.number],current->item.member.group_member_nickname);
            }
            current = current->next;
        }
        current = head_data;
        while(current != NULL)
        {
            if(current -> item.data.group_account == group)
            {
                strcpy(box.message_date[0],current->item.data.group_name);
                break;
            }
            current = current->next;
        }
        box.message_account[0] = group;
        PACK pack;
        pack.data.send_fd = 0;
        pack.data.recv_fd = 66;
        send(fd,&pack,sizeof(pack),0);

        send(fd,&box,sizeof(box),0);
        return 1;

    }
    /*
    char string[1024] = "\0";
    sprintf(string,"select group_account from group_member where group_member_account =%d",account);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    MYSQL_RES * result = mysql_store_result(&mysql);
    MYSQL_ROW row;
    int ret = 0; 
    while((row = mysql_fetch_row(result)) != NULL)
    {
        printf("%d\n",atoi(*row));
        if(group == atoi(*row))
        {
            ret = 1;
            break;
        }
    }
    if(ret == 0)
        return -1;
    memset(string,0,sizeof(string));
    sprintf(string,"select group_member_account,group_member_nickname,group_state from group_member where group_account = %d",group);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while((row = mysql_fetch_row(result)) != NULL)
    {
        box.number ++;
        box.message_account[box.number] = atoi(row[0]);
        box.type[box.number] = atoi(row[2]);
        strcpy(box.message_date[box.number],row[1]);
    }
    memset(string,0,sizeof(string));
    sprintf(string,"select group_name from group_member where group_account = %d",group);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    strcpy(box.message_date[0],*row);
    box.message_account[0] = group;


    PACK pack;
    pack.data.send_fd = 0;
    pack.data.recv_fd = 66;
    send(fd,&pack,sizeof(pack),0);

    send(fd,&box,sizeof(box),0);
    return 1;
*/
}


int Look_Group(int fd,int account)
{
    BOX box;
    memset(&box,0,sizeof(box));
    mysql_node *current;
    current = head_member;
    while(current != NULL)
    {
        if(current -> item.member.group_member_account == account)
        {
            box.number ++;
            box.message_account[box.number] = current->item.member.group_account;
            box.type[box.number] = current->item.member.group_state;
            strcpy(box.message_date[box.number],current->item.member.group_name);

        }
        current = current->next;
    }
    PACK pack;
    pack.data.recv_fd = 66;
    pack.data.send_fd = 0;
    send(fd,&pack,sizeof(pack),0);

    send(fd,&box,sizeof(box),0);
    return 0;
    /*
    char string[1024] = "\0";
    sprintf(string,"select group_account,group_name,group_state from group_member where group_member_account = %d",account);
    mysql_query(&mysql,string);
    MYSQL_RES * result = mysql_store_result(&mysql);
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        box.number ++;
        box.message_account[box.number] = atoi(row[0]);
        box.type[box.number] = atoi(row[2]);
        strcpy(box.message_date[box.number],row[1]);
    }

    PACK pack;
    pack.data.recv_fd = 66;
    pack.data.send_fd = 0;
    send(fd,&pack,sizeof(pack),0);

    send(fd,&box,sizeof(box),0);
    return 0;
*/
}


  //2  是群主不能退出群，只能解散群
  //1  退出成功
  //0 未加群
int Return_Group(int account,int group)
{
    int flag = 0;
    mysql_node *current;
    current = head_member;
    while(current != NULL)
    {
        if(current -> item.member.group_account == group && current->item.member.group_member_account == account)
        {
            flag = 1;
            if(current->item.member.group_state == 2 || current->item.member.group_state == 3)
            {
                flag = 2;
            }
            break;
        }
        current = current->next;
    }
    if(flag == 0)
        return 0;
    else if(flag == 1)
        return 2;
    else if(flag == 2)
    {
        mysql_node *prev,*current_group;
        current_group = head_member;
        while(current_group != NULL)
        {
            if(current_group->item.member.group_account == group && current_group->item.member.group_member_account == account)
            {
                break;
            }
            current_group = current_group->next;
        }
        if(current->next == NULL)
            prev->next = NULL;
        //printf("............\n");
        prev->next = current->next;
        //printf("............\n");
        //free(current);
        char string[1024] = "\0";
        sprintf(string,"delete from group_member where group_member_account = %d and group_account = %d",account,group);
        if(mysql_query(&mysql,string) == 0)
        {
            memset(server_buf,0,sizeof(server_buf));
            time_t ticks;
            ticks = time(NULL);
            sprintf(server_buf,"账户 %d 退出群聊%d\t\t\t%24s\n",account,group,ctime(&ticks));
            write(fp,server_buf,strlen(server_buf));
            return  1;
        }
        

    }
    /*
    char string[1024] = "\0";
    sprintf(string,"select group_state from group_member where group_member_account = %d and group_account = %d",account,group);
    mysql_query(&mysql,string);
    MYSQL_RES * result = mysql_store_result(&mysql);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    if(1 == atoi(*row))
        return 2;
    sprintf(string,"delete from group_member where group_member_account = %d and group_account = %d",account,group);
    if(mysql_query(&mysql,string) == 0)
    {
        memset(server_buf,0,sizeof(server_buf));
        time_t ticks;
        ticks = time(NULL);
        sprintf(server_buf,"账户 %d 退出群聊%d\t\t\t%24s\n",account,group,ctime(&ticks));
        write(fp,server_buf,strlen(server_buf));
    }

    return 1;
    */
}


//-1 未加入该群
//1  发送消息成功
int Group_Chat(PACK * pack)
{
    int flag = 0;
    mysql_node * current;
    current = head_member;
    while(current != NULL)
    {
        if(current->item.member.group_account == pack->data.send_account && current->item.member.group_member_account == pack->data.recv_account)
        {
            flag = 1;
            break;
        }
    }
    if(flag == 0)
        return -1;
    
    else 
    {
        int account;
        current = head_member;
        mysql_node *current_user;
        while(current != NULL)
        {
            if(current->item.member.group_account == pack->data.send_account && current->item.member.group_member_account != pack->data.recv_account)
            {
                account = current->item.member.group_member_account;
                current_user = head_user;
                while(current_user != NULL)
                {
                    if(current_user->item.user.account == account)
                    {
                        if(current_user->item.user.user_socket != 0)
                        {
                            pack->data.recv_fd = 0;
                            pack->data.send_fd = 1;
                            send(current_user->item.user.user_socket,pack,sizeof(*pack),0);
                        }
                    }
                }
                mysql_node * new_chat,current_chat;
                new_chat = (mysql_node *)malloc(sizeof(mysql_node));
                new_chat->item.chat.send_user = pack->data.send_account;
                new_chat->item.chat.recv_user = account;
                strcpy(new_chat->item.chat.messages,pack->data.write_buf);
                new_chat->item.chat.send_can_look = 1;
                new_chat->item.chat.recv_can_look = 1;
                new_chat->item.chat.message_type = 1;
                char nowTime[81] = "\0";
                memset(nowTime,0,sizeof(nowTime));
                getNowTime(nowTime);
                strcpy(new_chat->item.chat.date,nowTime);
                new_chat->next = NULL;
                mysql_list(&head_chat,new_chat);
                char string[1024] = "\0";
                sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",pack->data.send_account,account,pack->data.write_buf,1,1,1,nowTime);
                if(mysql_query(&mysql,string) == 0)
                {

                    memset(server_buf,0,sizeof(server_buf));
                     sprintf(server_buf,"群(%d)消息：%s（接收账户：%d）\t\t\t%24s\n",pack->data.send_account,pack->data.write_buf,account,nowTime);
                     write(fp,server_buf,strlen(server_buf));
                    return 1;
                }
            }
            current = current->next;

        }
    }
    /*
    char string[4096] = "\0";
    sprintf(string,"select group_account from group_member where group_member_account = %d",pack->data.recv_account);
    mysql_query(&mysql,string);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row ;
    int ret = 0;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        if(atoi(*row) == pack->data.send_account)
        {
            ret = 1;
            break;
        }
    }
    if(ret != 1)
        return -1;

    memset(string,0,sizeof(string));
    char nowTime[32] = {0};
    MYSQL_RES *result1;
    MYSQL_ROW row1;
    sprintf(string,"select group_member_account from group_member where group_account = %d",pack->data.send_account);
    printf("%s\n",string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while((row = mysql_fetch_row(result)) != NULL)
    {
        if(atoi(*row) == pack->data.recv_account)
            continue;
        memset(nowTime,0,sizeof(nowTime));
            getNowTime(nowTime);
        sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",pack->data.send_account,atoi(*row),pack->data.write_buf,1,1,1,nowTime);
        //printf("%s\n",string);
        memset(server_buf,0,sizeof(server_buf));
        sprintf(server_buf,"群(%d)消息：%s（接收账户：%d）\t\t\t%24s\n",pack->data.send_account,pack->data.write_buf,atoi(*row),nowTime);
        write(fp,server_buf,strlen(server_buf));
        if(mysql_query(&mysql,string) < 0)
        {
            printf("%d : send error!\n",__LINE__);
            continue;
        }
        memset(string,0,sizeof(string));
        sprintf(string,"select user_socket from user_data where account = %d",atoi(*row));
        //printf("%s\n",string);
        mysql_query(&mysql,string);
        result1 = mysql_store_result(&mysql);
        row1 = mysql_fetch_row(result1);
        if(atoi(*row1) != 0)
        {
            pack->data.recv_fd = 0;
            pack->data.send_fd = 1;
            send(atoi(*row1),pack,sizeof(*pack),0);
            continue;
        }
    }
    return 1;
    */
}

// 1 已加入   2 等待同意  -1 不存在
int Add_Group(int send_account,int group)
{
    int flag = 0;
    mysql_node * current;
    current = head_member;
    int qunzhu;
    while(current != NULL)
    {
        if(current -> item.member.group_account == group )
        {
            flag = 1;
            if(current -> item.member.group_member_account == send_account)
                flag = 2;
        }
        current = current -> next;
    }
    if(flag == 2)
        return 1;
    else if(flag == 0)
        return -1;
    else if(flag == 1)
    {
        current = head_data;
        while(current != NULL)
        {
            if(current->item.data.group_account == group)
            {
                qunzhu = current->item.data.group_number;
                break;
            }
            current = current->next;
        }
        current = head_user;
        while(current != NULL)
        {
            if(current -> item.user.account == qunzhu)
            {
                if(current->item.user.user_socket != 0)
                {
                    PACK pack;
                    pack.data.send_fd = 4;
                    pack.data.recv_fd = 0;
                    send(current->item.user.user_socket,&pack,sizeof(pack),0);
                }
            }
            current = current->next;
        }
        char string[1024] = "\0";
        char group_account[50] = "\0";
        char nowTime[81] = "\0";
        Get_Time(nowTime);
        sprintf(group_account,"%d",group);
        sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",send_account,qunzhu,group_account,1,1,4,nowTime);
        if(mysql_query(&mysql,string) == 0)
        {
            memset(server_buf,0,sizeof(server_buf));
            Get_Time(nowTime);
            sprintf(server_buf,"账户  ： %d 申请加入群 ： %d\t\t\t%24s\n",send_account,group,nowTime);
            write(fp,server_buf,strlen(server_buf));
            return 2;
        }
    }
    //查看群是否存在
    /*
    char string[1024] = "\0";
    strcpy(string,"select group_account from group_data\0");
    
    mysql_query(&mysql,string);
    printf("%d : %s\n",__LINE__,string);
    MYSQL_RES *result;
    int ret = 0;
    MYSQL_ROW row;
    result = mysql_store_result(&mysql);
    while((row = mysql_fetch_row(result)) != NULL)
    {
        if(atoi(*row) == group)
        {
            ret = 1;
            break;
        }
    }
    if(ret != 1)
        return -1;


    //查看该成员是否已经加入群
    memset(string,0,sizeof(string));
    sprintf(string,"select group_name from group_member where group_account = %d and group_member_account = %d",group,send_account);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    if(row != NULL)
        return 1;


    //找到群主
    memset(string,0,sizeof(string));
    sprintf(string,"select group_number from group_data where group_account = %d",group);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    int recv_account = atoi(*row);

    //发送给群主请求
    memset(string,0,sizeof(string));
    sprintf(string,"select user_socket from user_data where account = %d",recv_account);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);

    if(atoi(*row) != 0)
    {
        PACK pack;
        pack.data.send_fd = 4;
        pack.data.recv_fd = 0;
        send(atoi(*row),&pack,sizeof(pack),0);

    }

    //保存到消息表
    char nowTime[32] = {0};
    getNowTime(nowTime);
    memset(string,0,sizeof(string));
    char group_account[50] = "\0";
    sprintf(group_account,"%d",group);
    sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",send_account,recv_account,group_account,1,1,4,nowTime);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    memset(server_buf,0,sizeof(server_buf));
    Get_Time(nowTime);
    sprintf(server_buf,"账户  ： %d 申请加入群 ： %d\t\t\t%24s\n",send_account,group,nowTime);
    write(fp,server_buf,strlen(server_buf));
    return 2;
    */

}

void Get_Time(char *string)
{
    time_t ticks;
    ticks = time(NULL);
    strcpy(string,ctime(&ticks));
}

// -1 该用户已添加进群
// 1  添加进群成功
int Manage_Group(PACK *pack)
{
    int flag = 0;
    mysql_node *current;
    current = head_member;
    while(current != NULL)
    {
        if(current->item.member.group_account == atoi(pack->data.write_buf) && current->item.member.group_member_account == pack->data.send_account)
        {
            flag = 1;
            break;
        }
    }
    if(flag == 1)
        return -1;
    else 
    {
        mysql_node *new_group;
        new_group = (mysql_node *)malloc(sizeof(mysql_node));
        new_group -> item.member.group_account = atoi(pack->data.write_buf);
        new_group -> item.member.group_member_account = pack->data.send_account;
        current = head_user;
        //找出成员名
        while(current != NULL)
        {
            if(current->item.user.account == pack->data.send_account)
            {
                strcpy(new_group->item.member.group_member_nickname,current->item.user.nickname);
                break;
            }
            current = current->next;
        }
        current = head_data;
        //找出群名
        while(current != NULL)
        {
            if(current->item.data.group_account == new_group->item.member.group_account)
            {
                strcpy(new_group->item.member.group_name,current->item.data.group_name);
                break;
            }
            current = current->next;
        }
        new_group->item.member.group_state = 3;
        new_group->next = NULL;
        mysql_list(&head_member,new_group);
        char string[1024] = "\0";
        sprintf(string,"insert into group_member values(%d,'%s',%d,'%s',%d)"
    ,new_group -> item.member.group_account,new_group->item.member.group_name,new_group -> item.member.group_member_account,new_group->item.member.group_member_nickname,3);
        if(mysql_query(&mysql,string) == 0)
        {
            return 1;
        }
    }
    /*
    //找到群账户 群昵称
    char string[1024] = "\0";
    sprintf(string,"select group_state from group_member where group_account = %d and group_member_account = %d",atoi(pack->data.write_buf),pack->data.send_account);
    mysql_query(&mysql,string);
    MYSQL_RES * result1 = mysql_store_result(&mysql);
    MYSQL_ROW row1= mysql_fetch_row(result1);
    if(row1  != NULL)
        return -1;
    
    memset(string,0,sizeof(string));
    sprintf(string,"select group_account,group_name from group_data where group_number = %d",pack->data.recv_account);
    //printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    int group_account = atoi(row[0]);
    char group_nickname[50] = "\0";
    strcpy(group_nickname,row[1]);

    //找到用户昵称
    memset(string,0,sizeof(string));
    sprintf(string,"select nickname from user_data where account = %d",pack->data.send_account);
    //printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    char user_nickname[50] = "\0";
    strcpy(user_nickname,*row);


    //插入群信息表
    memset(string,0,sizeof(string));
    sprintf(string,"insert into group_member values(%d,'%s',%d,'%s',%d)",group_account,group_nickname,pack->data.send_account,user_nickname,3);
    //printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    
    memset(server_buf,0,sizeof(server_buf));
    time_t ticks;
    ticks = time(NULL);
    sprintf(server_buf,"同意账户 ： %d 加群(%d)成功\t\t\t%24s\n",pack->data.send_account,group_account,ctime(&ticks));
    write(fp,server_buf,strlen(server_buf));
    memset(string,0,sizeof(string));
    sprintf(string,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d and date = '%s'"
    ,pack->data.send_account,pack->data.recv_account,pack->data.recv_user);
    printf("%s\n",string);
    mysql_query(&mysql,string);
    return 1;
*/
    
}



//-1 群不存在   -2 不是群主  1 设置成功
int Set_Qun(PACK *pack)
{
    int flag = 0;
    mysql_node *current;
    current = head_member;
    while(current != NULL)
    {
        if(current ->item.member.group_account == pack->data.send_account && current -> item.member.group_member_account == pack->data.recv_account)
        {
            flag = 1;
            if( current->item.member.group_state == 1)
            {
                flag = 2;
                break;
            }
        }
        current = current->next;
    }
    if(flag == 1)
        return -2;
    else if(flag == 0)
        return -1;
    else if(flag == 2)
    {
        current = head_member;
        while(current != NULL)
        {
            if(current ->item.member.group_account == pack->data.send_fd)
            {
                current->item.member.group_state = pack->data.recv_fd;
                break;
            }
            current = current->next;
        }
        char string[1024] = "\0";
        sprintf(string,"update group_member set group_state = %d where group_account = %d and group_member_account = %d"
        ,pack->data.recv_fd,pack->data.send_account,pack->data.send_fd);
        if(mysql_query(&mysql,string) == 0)
        {
            time_t ticks;
            ticks = time(NULL);
            memset(server_buf,0,sizeof(server_buf));
            sprintf(server_buf,"设置账户 ： %d 为 %d（1.群主 2.管理员 3.群成员）\t\t\t%24s\n",pack->data.send_fd,pack->data.recv_fd,ctime(&ticks));
            write(fp,server_buf,strlen(server_buf));
            return 1;
        }
    }
    /*
    char string[1024] = "\0";
    sprintf(string,"select group_number from group_data where group_account = %d",pack->data.send_account);
    mysql_query(&mysql,string);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL)
    {
        return -1;
    }
    else if(atoi(*row) != pack->data.recv_account)
    {
        return -2;
    }
    else
    {
        memset(string,0,sizeof(string));
        sprintf(string,"update group_member set group_state = %d where group_account = %d and group_member_account = %d"
        ,pack->data.recv_fd,pack->data.send_account,pack->data.send_fd);
        mysql_query(&mysql,string);
        time_t ticks;
        ticks = time(NULL);
        memset(server_buf,0,sizeof(server_buf));
        sprintf(server_buf,"设置账户 ： %d 为 %d（1.群主 2.管理员 3.群成员）\t\t\t%24s\n",pack->data.send_fd,pack->data.recv_fd,ctime(&ticks));
        write(fp,server_buf,strlen(server_buf));
        return 1;
    }
    */
}
// 0 接收失败   1  接收成功  -1 不是好友
int Recv_File(int fd,PACK *pack)
{
    
    char buffer[1024] = "\0";
    char File_Name[100] = "\0";
    Cut_File(buffer,pack->data.send_user);
    strcpy(pack->data.send_user,buffer);
    memset(buffer,0,sizeof(buffer));
    printf("----------%s----------\n",pack->data.send_user);               
    sprintf(File_Name,"1%s",pack->data.send_user);
    //FILE * fp = fopen(File_Name,"w+");
    //fclose(fp);
    int fp = open(File_Name,O_CREAT  | O_TRUNC ,0666);
    if(fp < 0)
    {
        perror("open failed 1769");
    }
    close(fp);
    fp = open(File_Name,O_APPEND | O_WRONLY);
    if(fp < 0)
        perror("open failed");
    memset(buffer,0,sizeof(buffer));
    int length = 0;
    int write_length;
    int i = 0;
    //接收文件
     while( (length = recv(fd,buffer,sizeof(buffer),0) ) > 0 )
    {
        
       
        

        i++;
        printf("server recv : %d\n",length);
        if(strcmp(buffer,"exit25908") == 0)
            break;
        if(length <0)
        {
            printf("Recieve Data From client Failed\n");
            return 0;
        }
        //fp = fopen(File_Name,"a+");
        
         write_length = write(fp,buffer,length);
            /*            
        if(write_length != length)
        {
            printf("write failed\n");
            break;
            //return 0;
        }
        */
        memset(buffer,0,sizeof(buffer));
        //fclose(fp);
        
    }
    close(fp);
    printf("接受文件成功！\n");
    
    //查看好友关系
    
    printf("*******************************8\n");
    char string[1024] = "\0";
    sprintf(string,"select realtion from friends where user = %d and friend_user = %d",pack->data.send_account,pack->data.recv_account);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    MYSQL_RES *result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL)
    {
        printf("+++++++++++++++++++++\n");
        return -1;
    }
    printf("--------------------\n");
    int realtion = atoi(*row);
    int recv_can_look;
    if(realtion == -1)
    
         recv_can_look = 0;
    
    else 
        recv_can_look = 1;

        //插入消息表
    memset(string,0,sizeof(string));
    char nowTime[32] = {0};
        getNowTime(nowTime);
    sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",pack->data.recv_account,pack->data.send_account,File_Name,realtion,recv_can_look,2,nowTime);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    
    memset(server_buf,0,sizeof(server_buf));
    sprintf(server_buf,"账户 ： %d 发送给账户 ： %d 文件 ： %s\t\t\t%24s\n",pack->data.recv_account,pack->data.send_account,File_Name,nowTime);
    write(fp,server_buf,strlen(server_buf));
    //查看账户是否在线
    memset(string,0,sizeof(string));
    sprintf(string,"select user_socket from user_data where account = %d",pack->data.send_account);
    printf("%d : %s\n",__LINE__,string);
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    row = mysql_fetch_row(result);
    printf("user_socket = %d\n",atoi(*row));
    if(atoi(*row) != 0)
    {
        PACK pack1;
        pack1.data.send_fd = 2;
        pack1.data.recv_fd = 0;
        send(atoi(*row),&pack1,sizeof(pack1),0);
        
    }

    return 1;

}

int Send_File(int fd,PACK * pack)
{
    PACK pack1;
    pack1.data.recv_fd = 77;
    strcpy(pack1.data.recv_user,pack->data.recv_user);
    send(fd,&pack1,sizeof(pack1),0);
    char buffer[1024] = "\0";
    int fp = open(pack->data.recv_user,O_RDONLY);
    printf("file_name = %s\n",pack->data.recv_user);
    if(fp == -1)
    {
        printf("file not found\n");
        return -1;
    }

    else 
    {
        int i = 0;
        memset(buffer,0,sizeof(buffer));
        int file_block_length = 0;
        
        while(( file_block_length = read(fp,buffer,sizeof(buffer))) > 0)
        {
            i++;
            
            printf("file_block_length = %d\n",file_block_length);
            printf("server send : %d\n",i);
            if(send(fd,buffer,file_block_length,0) < 0)
            {
                printf("Send file failed\n");
                break;
            }
            
            memset(buffer,0,sizeof(buffer));

        }
        
        close(fp);
        memset(buffer,0,sizeof(buffer));
        sleep(1);
        strcpy(buffer,"exit25908");
        send(fd,buffer,strlen(buffer),0);
        
        printf("发送成功\n");
        remove(pack->data.recv_user);
        memset(server_buf,0,sizeof(server_buf));
        time_t ticks;
        ticks = time(NULL);
        sprintf(server_buf,"账户 : %d 接收文件%s成功(来自账户%d)\t\t\t",pack->data.recv_account,pack->data.recv_user,pack->data.send_account);
        write(fp,server_buf,strlen(server_buf));
        return 1;
    }

}


void Cut_File( char *string,char *filename)
{
    int k;
    int n = strlen(filename);
    printf("%d\n",n);
    for(int i = 0;i < n;i++)
    {
        if(filename[i] == '/')
        {
            
            k = 0;
        }
        k++;
       
    }
    int j = 0;
     printf("%d\n",k);
    for(int i = n-k+1;i < n;i++)
    {
        string[j] = filename[i];
        j++;
    }
    string[j] = '\0';
}
//0  chenggong
//-1 该账户无权管理群
//-2 该账户未加入群
//-3 无权踢管理员或群主
int Out_Member(int account,int group_account,int accounted)
{
    char string[1024] = "\0";
    //查看account是不是群主或管理员
    sprintf(string,"select group_state from group_member where group_member_account = %d and group_account = %d",account,group_account);
    mysql_query(&mysql,string);
    printf("%s\n",string);
    MYSQL_RES * result = mysql_store_result(&mysql);
    MYSQL_ROW row = mysql_fetch_row(result);
    if(row == NULL)
        return -1;
    if(atoi(*row) == 3)
        return -1;
    printf("%d\n",atoi(*row));
   
 
    //查看accounted在不在群里
        memset(string,0,sizeof(string));
        sprintf(string,"select group_state from group_member where group_member_account = %d and group_account = %d",accounted,group_account);
        printf("%s\n",string);
        mysql_query(&mysql,string);
        result = mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        if(row == NULL)
            return -2;
        if((atoi(*row) == 1 )|| (atoi(*row) == 2))
        {
            return -3;
        }

        //踢出好友
        memset(string,0,sizeof(string));
        sprintf(string,"delete from group_member where group_member_account = %d and group_account = %d",accounted,group_account);
        printf("%s\n",string);
        mysql_query(&mysql,string);
        memset(server_buf,0,sizeof(server_buf));
        time_t ticks;
        ticks = time(NULL);
        sprintf(server_buf,"账户%d 从群%d 踢出%d\t\t\t%24s\n",account,group_account,accounted,ctime(&ticks));
        write(fp,server_buf,strlen(server_buf));
        return 0;
   
}
void read_mysql_user_data(mysql_node **phead)
{
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;
    char string[1024] = "\0";
    mysql_node * current,*new_node;
    sprintf(string,"select * from user_data");
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result))
    {
        new_node = (mysql_node *)malloc(sizeof(mysql_node));
        new_node->item.user.account = atoi(row[0]);
        strcpy(new_node->item.user.nickname,row[1]);
        strcpy(new_node->item.user.password,row[2]);
        new_node->item.user.user_state = atoi(row[3]);
        new_node->item.user.user_socket = atoi(row[4]);
        strcpy(new_node->item.user.mibao,row[5]);
        strcpy(new_node->item.user.daan,row[6]);
        new_node->next = NULL;
        if(*phead == NULL)
        {
            *phead = new_node;

        }
        else
        {
            current->next = new_node;
        }
        current = new_node;
    }
}
void read_mysql_chat_messages(mysql_node ** phead)
{
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;
    char string[1024] = "\0";
    mysql_node * current,*new_node;
    sprintf(string,"select * from chat_messages");
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result))
    {
         new_node = (mysql_node *)malloc(sizeof(mysql_node));
         new_node->item.chat.send_user = atoi(row[0]);
         new_node->item.chat.recv_user = atoi(row[1]);
         strcpy(new_node->item.chat.messages,row[2]);
         new_node->item.chat.send_can_look = atoi(row[3]);
         new_node->item.chat.recv_can_look = atoi(row[4]);
         new_node->item.chat.message_type = atoi(row[5]);
         strcpy(new_node->item.chat.date,row[6]);
        new_node->next = NULL;
        if(*phead == NULL)
            *phead = new_node;

        else 
            current->next = new_node;
    
        current = new_node;
    }
}
void read_mysql_friends(mysql_node ** phead)
{
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;
    char string[1024] = "\0";
    mysql_node * current,*new_node;
    sprintf(string,"select * from friends");
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result))
    {
        new_node = (mysql_node * )malloc(sizeof(mysql_node));
        new_node->item.friends.user = atoi(row[0]);
        new_node->item.friends.friend_user = atoi(row[1]);
        new_node->item.friends.realtion = atoi(row[2]);
        new_node->next = NULL;
        if(*phead == NULL)
            *phead = new_node;

        else 
            current->next = new_node;
    
        current = new_node;
    }
}

void read_mysql_group_data(mysql_node ** phead)
{
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;
    char string[1024] = "\0";
    mysql_node * current,*new_node;
    sprintf(string,"select * from group_data");
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result))
    {
        new_node = (mysql_node * )malloc(sizeof(mysql_node));
        new_node->item.data.group_account = atoi(row[0]);
        strcpy(new_node->item.data.group_name,row[1]);
        new_node->item.data.group_number = atoi(row[2]);
        new_node->next = NULL;
        if(*phead == NULL)
            *phead = new_node;
 
        else 
            current->next = new_node;
    
        current = new_node;
    }
}
void read_mysql_group_member(mysql_node ** phead)
{
    MYSQL_RES * result = NULL;
    MYSQL_ROW row;
    char string[1024] = "\0";
    mysql_node * current,*new_node;
    sprintf(string,"select * from group_member");
    mysql_query(&mysql,string);
    result = mysql_store_result(&mysql);
    while(row = mysql_fetch_row(result))
    {
        new_node = (mysql_node * )malloc(sizeof(mysql_node));
        new_node -> item.member.group_account = atoi(row[0]);
        strcpy(new_node->item.member.group_name,row[1]);
        new_node->item.member.group_member_account = atoi(row[2]);
        strcpy(new_node->item.member.group_member_nickname,row[3]);
        new_node->item.member.group_state = atoi(row[4]);
        new_node->next = NULL;
        if(*phead == NULL)
            *phead = new_node;

        else 
            current->next = new_node;
    
        current = new_node;
    }
}

void mysql_list(mysql_node ** phead,mysql_node *new_node)
{
    mysql_node * current;
    if(*phead == NULL)
    {
        *phead = new_node;
        return;
    }
    current = * phead;
    while(current->next != NULL)
        current = current->next;

    current -> next = new_node;
    
    
}