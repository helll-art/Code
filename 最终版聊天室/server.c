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
#define INTO_CHAT_UI 121
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
int Manage_Friend(int recv_account,int send_account,int flag );
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

char * Into_Chat_UI(int account);

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
                        send(events[i].data.fd,&pack,sizeof(pack),0);
                        continue;
                    }
                }
                else if(pack.type == CHANGE_PASSWORD)
                {

                    int ret_CP = -1;
                    ret_CP = Change_Password(&pack,&mysql);
                    send(events[i].data.fd,&ret_CP,sizeof(ret_CP),0);
                    continue;


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
                    printf("pack.data.ret_AAA = %d\n",pack.data.ret_AAA);
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
                    if(pack.data.recv_fd == 1 || pack.data.recv_fd == 0)
                    {
                        pack.data.ret_AAA = Manage_Friend(pack.data.recv_account,pack.data.send_account,pack.data.recv_fd);
                       
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
                    printf("ret = %d\n",pack.data.ret_AAA);
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
                else if(pack.type == INTO_CHAT_UI)
                {
                    char *nickname ;
                    nickname = Into_Chat_UI(pack.data.send_account);
                    
                    if(nickname != NULL)
                    {
                        strcpy(pack.data.write_buf,nickname);
                    }
                    else 
                        strcpy(pack.data.write_buf,"该用户不存在，请退出..");
                    pack.data.recv_fd = 88;
                    pack.data.send_fd = 0;
                    send(events[i].data.fd,&pack,sizeof(pack),0);
                    continue;
                }
              
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
    /*
    current = head_user;
    while(current != NULL)
        current = current -> next;
*/
    mysql_list(&head_user,new_user);
    current = head_user;
    while(current != NULL)
    {
        printf("账户名： %d 密码：%s\n",current -> item.user.account,current -> item.user.password);
        current  = current -> next;
    }
    char string[4096] = "\0";
    sprintf(string,"insert into user_data values(%d,'%s','%s',0,0,'%s','%s')",account,pack->data.recv_user,pack->data.send_user,pack->data.read_buf,pack->data.write_buf);
    
    if((mysql_query(mysql,string)) == 0)
    {
        memset(server_buf,0,sizeof(server_buf));
        sprintf(server_buf,"申请新账户\n 账户：%d 账户名：%s 密码：%s  密保：%s 答案: %s\n"
        ,account,pack->data.recv_user,pack->data.send_user,pack->data.read_buf,pack->data.write_buf);
        write(fp,server_buf,strlen(server_buf));
        current = head_user;
        
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

        return -1;

    
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
            strcpy(current -> item.user.password,pack->data.send_user);
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
            //以添加为好友
            flag = 2;
            break;
        }
        current = current -> next;
    }
    current = head_user;
    while(current != NULL)
    {
        //printf("%d\t%d\n",current->item.user.account,current->item.user.user_socket);
        if(current->item.user.account == pack->data.recv_account)
        {
            
            if(current->item.user.user_state == 1)
            {
                online_flag = 1;
          //      printf("%d\n%d\n",current->item.user.account,current->item.user.user_socket);
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
        new_requst = head_chat;
        
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

    
}

//设计思路：
//0 黑名单
//1 发送完成
//-1 不是好友关系
//不是好友关系 返回-1  是好友发送消息 返回0
int Signal_Chat(PACK *pack)
{
    printf("Signal_Chat\n");
    int flag = 0;
    mysql_node * current;
    current = head_friends;
    //查看发送者对接受者的关系
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
    printf("realiton =  %d\n",flag);
    int realtion = 0;
    //把该好友加入黑名单不能发送消息
    if(flag == 1)
        return 0;
    //不是好友
    else if(flag == 0)
        return -1;
    //发送者对接受者是好友
    else if(flag == 2)
    {
        printf("进入\n");
         printf("user = %d\t friend_user = %d realtion = %d\n",current -> item.friends.user,current -> item.friends.friend_user,current -> item.friends.realtion);
        mysql_node * current_friend;
        current_friend = head_friends;
        //查看接受者对发送者的关系
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
        printf("接受者： %d 发送者： %d realtion = %d\n",pack->data.recv_account,pack->data.send_account,realtion);
        if(realtion == 1 || realtion == 2 )
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
            printf("%d %d %s %d %d %d %s\n",new_messages -> item.chat.send_user,new_messages ->item.chat.recv_user,
            new_messages -> item.chat.messages,new_messages->item.chat.send_can_look,new_messages->item.chat.recv_can_look,new_messages->item.chat.message_type,new_messages->item.chat.date);
            mysql_list(&head_chat,new_messages);
            
            printf("插入chat链表成功\n");
            current = head_chat;

            while(current != NULL)
            {
                printf("%d %d %s %d %d %d %s\n",
        current -> item.chat.send_user,current -> item.chat.recv_user,current->item.chat.messages,
        current->item.chat.send_can_look,current->item.chat.recv_can_look,current->item.chat.message_type,current->item.chat.date);
                
                current = current -> next;
            }
            printf("nihaohhhhhh\n");
            current = head_user;
            int fd = 0;
            //查看接受者是否在线
            while(current != NULL)
            {
                if(current ->item.user.account == pack->data.recv_account)
                {
                    pack->data.recv_fd = 99;
                    fd = current -> item.user.user_socket;
                    printf("fd = %d\n",fd);
                }
                current = current->next;
            }
            printf("fd = %d\n",fd);
            current = head_user;
            while(current != NULL)
            {
                if(current -> item.user.account == pack->data.recv_account)
                {
                    strcpy(pack->data.send_user,current -> item.user.nickname);
                    break;
                }
                current = current -> next;
            }
            if(fd != 0)
            {
                pack->data.send_fd = 1;
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
        else 
            return 1;
       
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
    printf("Del_Friend\n");
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
    if(user_flag == 0)
    {
        return -1;
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
        if(current == head_friends)
            head_friends = current -> next;
        //printf("............\n");
        else 
            prev->next = current->next;
        //printf("............\n");
        //free(current);
        char string[1024] = "\0";
        //printf("dengdaishanchu\n");
        current = head_friends;
        printf("-------------------------friends---------------------\n");
        while(current != NULL)
        {
            printf("user = %d,friend_user = %d,realtion = %d\n",current -> item.friends.user,current -> item.friends.friend_user,current -> item.friends.realtion);
            current = current -> next;
        }
        printf("------------------------------------------------------\n");
        sprintf(string,"delete from friends where friend_user = %d and user = %d",pack->data.recv_account,pack->data.send_account);
        mysql_query(mysql,string);
        return 0;

    }
    else if(flag == 0)
    {
        return 1;
    }
    
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
   
    send(fd,&i,sizeof(i),0);
    return 0;
}

int Look_MessageBox(int fd,int account)
{

    printf("Look MessageBox\n");
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
        
        if(current->item.chat.recv_user == account && (current->item.chat.send_can_look == 1 || current->item.chat.send_can_look == 2))
        {
            box.number ++;
            box.type[box.number] = current->item.chat.message_type;
            if(box.type[box.number] == 1)       //xiaoxi
            {
                printf("xiaoxi\n");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.talk_number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 2)      //wenjian
            {
                printf("wenjian\n");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.talk_number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 3)      //haoyouqingqiu
            {
                printf("haoyou\n");
                box.friend_number ++;
                box.plz_account[box.friend_number] = current->item.chat.send_user;
                strcpy(box.plz_date[box.friend_number],current->item.chat.date);
                printf("%d  : %s\n",box.plz_account[box.friend_number],box.plz_date[box.friend_number]);
            }
            else if(box.type[box.number] == 4)   //qunliao
            {
                printf("qunliao\n");
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
   
}

int Manage_Friend(int recv_account,int send_account,int flag)
{
    if(flag == 1)
    {
        printf("Manage Friend\n");
        mysql_node * current,*new_friend;
        
        new_friend = (mysql_node *)malloc(sizeof(mysql_node));
        new_friend ->item.friends.user = recv_account;
        new_friend -> item.friends.friend_user = send_account;
        new_friend -> item.friends.realtion = 1;
        new_friend -> next = NULL;
        mysql_list(&head_friends,new_friend);    
        mysql_node * new_friend1;
        new_friend1 = (mysql_node *)malloc(sizeof(mysql_node));
        new_friend1 ->item.friends.user = send_account;
        new_friend1 -> item.friends.friend_user = recv_account;
        new_friend1 -> item.friends.realtion = 1;
        new_friend1 -> next = NULL;
        
        mysql_list(&head_friends,new_friend1);

        current = head_friends;
        
        while(current != NULL)
        {
            printf("%d %d %d\n",current->item.friends.user,current->item.friends.friend_user,current->item.friends.realtion);
            current = current->next;
        }
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
        current = head_chat;
        while(current != NULL)
        {
            if(current -> item.chat.send_user == send_account && current -> item.chat.recv_user == recv_account && strcmp(current->item.chat.messages,"好友请求") == 0)
            {
                current -> item.chat.send_can_look = 0;
                break;
            }
            current = current -> next;
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
    else 
    {
        printf("拒绝好友请求\n");
        mysql_node *current;
        current = head_chat;
        while(current != NULL)
        {
            if(current -> item.chat.send_user == send_account && current -> item.chat.recv_user == recv_account && strcmp(current->item.chat.messages,"好友请求") == 0)
            {
                current -> item.chat.send_can_look = 0;
                break;
            }
            current = current -> next;
        }
        char string[1024] = "\0";
        sprintf(string,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d and messages = '好友请求'",send_account,recv_account);
        
        if(mysql_query(&mysql,string) < 0)
        {
            printf("拒绝失败\n");
        }
        return 1;
    }
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
    printf("send_account = %d\n",send_account);
    printf("recv_account = %d\n",recv_account);
    printf("string = %s\n",string);
    mysql_node * current;
    current = head_chat;
    while(current != NULL)
    {
        if(current -> item.chat.recv_user == send_account && current -> item.chat.send_user == recv_account && (strcmp(current -> item.chat.messages,string) == 0))
        {
            current -> item.chat.send_can_look = 0;
            break;
        }
        current = current -> next;
    }
    current = head_chat;
    printf("                                 Manage_file  chat_Messages\n");
    while(current != NULL)
    {
        printf("%d %d %s %d %d %d %s\n",
            current -> item.chat.send_user,current -> item.chat.recv_user,current -> item.chat.messages,current -> item.chat.send_can_look,
            current -> item.chat.recv_can_look,current -> item.chat.message_type ,current -> item.chat.date);
        current = current -> next;
    }
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("Manage file\n");
    char string1[1024] = "\0";
    sprintf(string1,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d and messages = '%s'"
    ,send_account,recv_account,string);
    printf("%s\n",string1);
    mysql_query(&mysql,string1);
}
int Look_Chat_Record(int fd,int account)
{
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
        
        if(current->item.chat.recv_user == account && (current->item.chat.recv_can_look == 1 || current->item.chat.send_can_look == 2))
        {
            box.number ++;
            box.type[box.number] = current->item.chat.message_type;
            if(box.type[box.number] == 1)       //xiaoxi
            {
                printf("xiaoxi\n");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.talk_number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 2)      //wenjian
            {
                printf("wenjian\n");
                box.talk_number ++;
                box.message_account[box.talk_number] = current->item.chat.send_user;
                strcpy(box.message_date[box.talk_number],current->item.chat.date);
                strcpy(box.messages[box.talk_number],current->item.chat.messages);
            }
            else if(box.type[box.number] == 3)      //haoyouqingqiu
            {
                printf("haoyou\n");
                box.friend_number ++;
                box.plz_account[box.friend_number] = current->item.chat.send_user;
                strcpy(box.plz_date[box.friend_number],current->item.chat.date);
                printf("%d  : %s\n",box.plz_account[box.friend_number],box.plz_date[box.friend_number]);
            }
            else if(box.type[box.number] == 4)   //qunliao
            {
                printf("qunliao\n");
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
   
}     


 
//-1   不是好友   1 设置成功 
int Set_Realtion(int send_account,int recv_account,int state)
{
    int flag = 0;
    mysql_node *current;
    current = head_friends;
    while(current != NULL)
    {
        if(current -> item.friends.user == send_account && current -> item.friends.friend_user == recv_account)
        {
            flag = 1;
            current->item.friends.realtion = state;
            char string[1024] = "\0";
            sprintf(string,"update friends set realtion = %d where user = %d and friend_user = %d",state,send_account,recv_account);
            mysql_query(&mysql,string);
            printf("send_account == %d recv_account == %d state == %d\n",current->item.friends.user,current->item.friends.friend_user,current->item.friends.realtion);
            return 1;
        }
        current = current -> next;
    }
    if(flag == 0)
        return -1;

}

int Registed_Group(PACK *pack)
{
    mysql_node * current,*new_group;
    current = head_data;
    
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
    new_group = (mysql_node *)malloc(sizeof(mysql_node));
    new_group->item.data.group_account = group_account;
    new_group->item.data.group_number = pack->data.send_account;
    strcpy(new_group->item.data.group_name,pack->data.send_user);
    new_group -> next = NULL;
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
    
        
   
}

//-1 不在群内
//0 无权解散群
//1 解散成功
int Free_Group(int account,int group)
{
    printf("Free Group\n");

    int flag = 0;
    mysql_node * current;
    current = head_member;
    while(current != NULL)
    {
        if(current->item.member.group_account ==  group && current->item.member.group_member_account == account)
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
        mysql_node * prev;
        prev = head_member;
        
        while(current != NULL)
        {
            if(current->item.member.group_account == group)
            {
                if(current == head_member)
                    head_member = current -> next;
                else 
                    prev -> next = current -> next;
            }
            prev = current;
            current = current -> next;
        }
        
        
        printf("...............\n");
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
         printf("--------------------------------------------group_data--------------------------------------------------------\n");
        current = head_data;
        while(current != NULL)
        {
            printf("%d %s %d\n",current -> item.data.group_account,current -> item.data.group_name,current -> item.data.group_number);
            current = current -> next;
        }
        current = head_data;
        prev = head_data;
        printf("删除data节点\n");
        while(current != NULL)
        {
            if(current->item.data.group_account == group)
                break;
            prev = current;
            current = current->next;
        }
        if(current == head_data)
            head_data = current->next;
        else 
            prev->next = current->next;
        printf("删除data节点成功\n");
        
        memset(string,0,sizeof(string));
        sprintf(string,"delete from group_data where group_number = %d and group_account = %d",account,group);
        mysql_query(&mysql,string);
        current = head_member;
        printf("AAAAAAAAAAAAAAAAAAAAAAA\n");
        printf("--------------------------------------------group_member------------------------------------------------------\n");
        while(current != NULL)
        {
            printf("%d %s %d %s \n",current -> item.member.group_account ,current -> item.member.group_name,current -> item.member.group_member_account ,current -> item.member.group_name);
            current = current -> next;
        }
        printf("--------------------------------------------group_data--------------------------------------------------------\n");
        current = head_data;
        while(current != NULL)
        {
            printf("%d %s %d\n",current -> item.data.group_account,current -> item.data.group_name,current -> item.data.group_number);
            current = current -> next;
        }
        return 1;
    }
   
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
   
}


  //2  是群主不能退出群，只能解散群
  //1  退出成功
  //0 未加群
int Return_Group(int account,int group)
{
    printf("Return Group\n");
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
    printf("------------flag = %d\n",flag);
    if(flag == 0)
        return 0;
    else if(flag == 1)
        return 2;
    else if(flag == 2)
    {

        mysql_node *prev,*current_group;
        current_group = head_member;
        prev = head_member;
        while(current_group != NULL)
        {
            if(current_group->item.member.group_account == group && current_group->item.member.group_member_account == account)
            {
                break;
            }
            prev = current_group ;
            current_group = current_group->next;
        }
        if(current_group == head_member)
            head_member = current_group -> next;
        
        else 
            prev->next = current_group->next;

        printf("------------------------group_member------------------------------\n");
        current = head_member;
        while(current != NULL)
        {
            printf("%d %s %d %s %d\n",current -> item.member.group_account,current -> item.member.group_name,current -> item.member.group_member_account,current -> item.member.group_member_nickname,current -> item.member.group_state);
            current = current -> next;
        }
        printf("-----------------------------------------------------------------\n");
        char string[1024] = "\0";
        sprintf(string,"delete from group_member where group_member_account = %d and group_account = %d",account,group);
        printf("string = %s\n",string);
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
   
}


//-1 未加入该群
//1  发送消息成功
int Group_Chat(PACK * pack)
{
    printf("Group Chat\n");
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
        current = current -> next;
    }
    printf("Flag = %d\n",flag);
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
                            printf("发送包\n");
                            pack->data.recv_fd = 0;
                            pack->data.send_fd = 1;
                            send(current_user->item.user.user_socket,pack,sizeof(*pack),0);
                        }
                    }
                    current_user = current_user -> next;
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
                printf("%s\n",string);
                if(mysql_query(&mysql,string) == 0)
                {

                    memset(server_buf,0,sizeof(server_buf));
                     sprintf(server_buf,"群(%d)消息：%s（接收账户：%d）\t\t\t%24s\n",pack->data.send_account,pack->data.write_buf,account,nowTime);
                     write(fp,server_buf,strlen(server_buf));
                    
                }
            }
            current = current->next;
            
        }
        return 1;
    }
   
}

// 1 已加入   2 等待同意  -1 不存在
int Add_Group(int send_account,int group)
{
    printf("Add Group...\n");
    int flag = 0;
    mysql_node * current;
    current = head_member;
    int qunzhu;
    //查看申请的群是否存在，并查看用户是否加入群
    while(current != NULL)
    {
        if(current -> item.member.group_account == group )
        {
            flag = 1;
            if(current -> item.member.group_member_account == send_account)
            {
                flag = 2;
                break;
            }
        }
        current = current -> next;
    }
    printf("flag = %d\n",flag);
    if(flag == 2)
        return 1;
    else if(flag == 0)
        return -1;
    else if(flag == 1)
    {
        mysql_node * current_AAA;
        mysql_node * current_user;
        current_AAA = head_member;
        while(current_AAA != NULL)
        {
            if(current_AAA -> item.member.group_account == group && (current_AAA -> item.member.group_state == 1 || current_AAA -> item.member.group_state == 2))
            {
                qunzhu = current_AAA -> item.member.group_member_account;
                printf("qunzhu = %d\n",qunzhu);
                current_user = head_user;
                while(current_user != NULL)
                {
                    if(current_user -> item.user.account == qunzhu)
                    {
                        if(current_user -> item.user.user_socket != 0)
                        {
                            PACK pack;
                            pack.data.send_fd = 4;
                            pack.data.recv_fd = 0;
                            send(current_user->item.user.user_socket,&pack,sizeof(pack),0);
                        }
                        break;
                    }
                    current_user = current_user -> next;
                }
                printf("1..................\n");
                char nowTime[81] = "\0";
                getNowTime(nowTime);
                current = head_chat;
                mysql_node * new_group_member;
                new_group_member = (mysql_node *)malloc(sizeof(mysql_node));
                new_group_member -> item.chat.send_user = send_account;
                new_group_member -> item.chat.recv_user = qunzhu;
                char group_account_string[24] = "\0";
                sprintf(group_account_string,"%d",group);
                strcpy(new_group_member -> item.chat.messages,group_account_string);
                new_group_member -> item.chat.send_can_look = 1;
                new_group_member -> item.chat.recv_can_look = 1;
                new_group_member -> item.chat.message_type = 4;
                strcpy(new_group_member -> item.chat.date,nowTime);
                new_group_member -> next = NULL;
                mysql_list(&head_chat,new_group_member);
                char string[1024] = "\0";
                char group_account[50] = "\0";
                memset(nowTime,0,sizeof(nowTime));
                getNowTime(nowTime);
                sprintf(group_account,"%d",group);
                sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",send_account,qunzhu,group_account,1,1,4,nowTime);
                printf("%s\n",string);
                if(mysql_query(&mysql,string) == 0)
                {
                    memset(server_buf,0,sizeof(server_buf));
                    Get_Time(nowTime);
                    sprintf(server_buf,"AAA账户  ： %d 申请加入群 ： %d\t\t\t%24s\n",send_account,group,nowTime);
                    write(fp,server_buf,strlen(server_buf));
                    
                }
            }
            current_AAA = current_AAA -> next;

        }
    }
        return 2;
      

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
    printf("Manage group\n");
    int flag = 0;
    mysql_node *current;
    current = head_member;
    printf("group_account = %d\n",atoi(pack->data.write_buf));
    while(current != NULL)
    {
        if(current->item.member.group_account == atoi(pack->data.write_buf) && current->item.member.group_member_account == pack->data.send_account)
        {
            flag = 1;
            break;
        }
        current = current -> next;
    }
    printf("flag = %d\n",flag);
    if(flag == 1)
        return -1;
    else 
    {
      
        printf("开始添加\n");
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
        current = head_chat;
        while(current != NULL)
        {
            if(strcmp(current -> item.chat.date,pack->data.recv_user) == 0 && (current -> item.chat.recv_user == pack->data.recv_account)
             && (current -> item.chat.send_user == pack->data.send_account))
            {
                current -> item.chat.send_can_look = 0;
                break;
            }
             current = current -> next;
        }
        current = head_chat;
        while(current != NULL)
        {
            printf("%d %d %d %d\n",current -> item.chat.send_user,current -> item.chat.recv_user,current -> item.chat.message_type,current -> item.chat.send_can_look);
            current = current -> next;
        }
        current = head_member;
        printf("----------------------------group_member------------------------------------------------------------\n");
        while(current != NULL)
        {
            printf("%d %s %d %s %d\n",current -> item.member.group_account,current -> item.member.group_name,
            current -> item.member.group_member_account,current -> item.member.group_member_nickname,
            current -> item.member.group_state);
            current = current -> next;
        }
        printf("-----------------------------------------------------------------------------------------------------\n");
        char string[1024] = "\0";
        sprintf(string,"update chat_messages set send_can_look = 0 where send_user = %d and recv_user = %d and date = '%s'"
    ,pack->data.send_account,pack->data.recv_account,pack->data.recv_user);
        mysql_query(&mysql,string);
        memset(string,0,sizeof(string));
        sprintf(string,"insert into group_member values(%d,'%s',%d,'%s',%d)"
    ,new_group -> item.member.group_account,new_group->item.member.group_name,new_group -> item.member.group_member_account,new_group->item.member.group_member_nickname,3);
        if(mysql_query(&mysql,string) == 0)
        {
            return 1;
        }
    }
    
    
}


char * Into_Chat_UI(int account) 
{
    printf("into chatUI\n");
    printf("%d\n",account);
    mysql_node * current;
    current = head_user;
    while(current != NULL)
    {
        if(current -> item.user.account == account)
        {
            char * string = NULL;
            string = (char *)malloc(sizeof(char) * 81);
            strcpy(string,current -> item.user.nickname);
            
            sprintf(string,"%s(%d)",current -> item.user.nickname,account);
            
            return string;
        }
        current = current -> next;
    }
    return NULL;
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
        printf("group_member_account = %d group_state = %d\n",pack->data.send_fd,pack->data.recv_fd);
        while(current != NULL)
        {
            if(current ->item.member.group_member_account == pack->data.send_fd && current -> item.member.group_account == pack->data.send_account )
            {
                current->item.member.group_state = pack->data.recv_fd;
                break;
            }
            current = current->next;
        }
        current = head_member;
        printf("--------------------------------------group_data--------------------------------------\n");
        while(current != NULL)
        {
            printf("%d %d %d\n",current -> item.member.group_account ,current -> item.member.group_member_account ,current -> item.member.group_state);
            current = current -> next;
        }
        printf("--------------------------------------------------------------------------------------\n");
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
   
}
// 0 接收失败   1  接收成功  -1 不是好友   -2 加入黑名单
int Recv_File(int fd,PACK *pack)
{
    
        int flag = 0;
        int realtion = 0;
        mysql_node * current ;
        current = head_friends;
        printf(".............\n");
        
        while(current != NULL)
        {
            if(current -> item.friends.user == pack->data.send_account && current -> item.friends.friend_user == pack->data.recv_account)
            {
                flag = 1;
                if(current -> item.friends.realtion == 1 || current -> item.friends.realtion == 2)
                {
                    realtion = current -> item.friends.realtion;
                    flag = 2;
                
                }
                break;
            }
            current = current -> next;
        }
        printf("flag == %d\n",flag);
        if(flag == 0)
            return -1;
        else if(flag == 1)
            return -2;
        char buffer[100] = "\0";
        char File_Name[100] = "\0";
        printf("pack->data.send_user = %s\n",pack -> data.send_user);
        pack -> data.send_user[strlen(pack -> data.send_user) ] = '\0';
        
        int n = strlen(pack -> data.send_user);
        int k = -1;
        for(int i = 0;i < n;i++)
        {
            k ++;
            if(pack -> data.send_user[i] == '/')
                k = -1;
        }
        int j = 0;
        for(int i = n - k - 1; i < n;i++)
            buffer[j++] = pack -> data.send_user[i];
        printf("buffer = %s\n",buffer);
        buffer[j] = '\0';
        printf("pack -> data.send_user = %s\n",pack -> data.send_user);
        printf("buffer = %s\n",buffer);
        strcpy(pack->data.send_user,buffer);

        printf("flag = %d\n",flag);
        memset(buffer,0,sizeof(buffer));
        printf("----------%s----------\n",pack->data.send_user);               
        sprintf(File_Name,"1%s",pack->data.send_user);
        //FILE * fp = fopen(File_Name,"w+");
        //fclose(fp);
        printf(".......AAAA.......\n");
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
        printf("开始接受文件...\n");
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
        mysql_node * new_file;
        char nowTime[32] = {0};
        getNowTime(nowTime);
        new_file = (mysql_node *)malloc(sizeof(mysql_node));
        new_file -> item.chat.send_user = pack->data.recv_account;
        new_file -> item.chat.recv_user = pack->data.send_account;
        strcpy(new_file -> item.chat.messages,File_Name);
        new_file -> item.chat.send_can_look = realtion;
        new_file -> item.chat.recv_can_look = 1;
        new_file -> item.chat.message_type = 2;
        strcpy(new_file -> item.chat.date,nowTime);
        new_file -> next = NULL;
        mysql_list(&head_chat,new_file);
        char string[1024] = "\0";
        sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",pack->data.recv_account,pack->data.send_account,File_Name,realtion,1,2,nowTime);
        printf("%d : %s\n",__LINE__,string);
        if(mysql_query(&mysql,string) != 0)
            return 0;
        
        memset(server_buf,0,sizeof(server_buf));
        sprintf(server_buf,"账户 ： %d 发送给账户 ： %d 文件 ： %s\t\t\t%24s\n",pack->data.recv_account,pack->data.send_account,File_Name,nowTime);
        write(fp,server_buf,strlen(server_buf));

        current = head_chat;
        while(current != NULL)
        {
            printf("%d %d %s %d %d %d %s\n",current -> item.chat.send_user,current -> item.chat.recv_can_look,current -> item.chat.messages,
            current -> item.chat.send_can_look,current -> item.chat.recv_can_look,current -> item.chat.message_type,current -> item.chat.date);
            current = current -> next;
        }
        current = head_user;
        while(current != NULL)
        {
            if(current -> item.user.account == pack->data.send_account)
            {
                if(current -> item.user.user_socket != 0)
                {
                    PACK pack1;
                    pack1.data.send_fd = 2;
                    pack1.data.recv_fd = 0;
                    send(current -> item.user.user_socket,&pack1,sizeof(pack1),0);
                    
                    return 1;
                }
            }
            current = current -> next;
        }

    
    return 1;
   
}

int Send_File(int fd,PACK * pack)
{
    if(pack -> data.send_fd == 1)
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
            Manage_File(pack -> data.recv_account,pack -> data.send_account,pack -> data.recv_user);
            return 1;
        }
    }
    Manage_File(pack  -> data.recv_account,pack->data.send_account,pack->data.recv_user);
    return 1;

}


void Cut_File( char *string,char *filename)
{
    int k;
    int n = strlen(filename);
    //printf("%d\n",n);
    printf("file name = %s\n",filename);
    for(int i = 0;i < n;i++)
    {
        if(filename[i] == '/')
        {
            
            k = -1;
        }
        k++;
       
    }
    int j = 0;
     //printf("%d\n",k);
    for(int i = n-k+1;i < n;i++)
    {
        string[j] = filename[i];
        j++;
    }
    string[j] = '\0';
    printf("string = %s\n",*string);
}
//0  chenggong
//-1 该账户无权管理群
//-2 该账户未加入群
//-3 无权踢管理员或群主
int Out_Member(int account,int group_account,int accounted)
{
    printf("Out Member\n");
    int flaged = 0,flag = 0 ,qunguan_flag = 0;
    mysql_node * current ;
    current = head_member;
    while(current != NULL)  
    {
        if(current -> item.member.group_account == group_account && current -> item.member.group_member_account == accounted)
        {
            if(current -> item.member.group_state == 1 || current -> item.member.group_state == 2)
            {
                qunguan_flag = 1;
                break;
            }
        }
        current = current -> next;
    }
    printf("qunguan_flag = %d\n",qunguan_flag );
    if(qunguan_flag == 1)
        return -3;
    current = head_member;
    while(current != NULL)
    {
        if(current -> item.member.group_account == group_account && current -> item.member.group_member_account == accounted)
        {
            flaged = 1;
            break;
        }
        current = current -> next;
    }
    current = head_member;
    while(current != NULL)
    {
        if(current -> item.member.group_account == group_account && current -> item.member.group_member_account == account)
        {
            flag = 1;
            if(current -> item.member.group_state == 1 || current -> item.member.group_state == 2)
            {
                flag = 2;
            }
            break;
        }
        current = current -> next;
    }
    printf("flag = %d\n",flag);
    if(flag == 1)
        return -1;
    else if(flaged == 0)
        return -2;
    else if(flag == 2)
    {
        mysql_node * prev ;
        prev = head_member;
        current = head_member;
        while(current != NULL)
        {
            if(current -> item.member.group_account == group_account && current -> item.member.group_member_account == accounted)
            {
                if(current == head_member)
                    head_member = current -> next;
                else 
                    prev->next = current -> next;
                break;
            }
            prev = current ;
            current = current -> next;
        }
        char string[1024] = "\0";
        sprintf(string,"delete from group_member where group_member_account = %d and group_account = %d",accounted,group_account);
        if(mysql_query(&mysql,string) == 0)
        {
            mysql_node * current_user;
            current_user = head_user;
            while(current_user != NULL)
            {
                if(current_user -> item.user.account == accounted)
                {
                    
                    if(current_user -> item.user.user_socket != 0)
                    {
                        PACK pack;
                        pack.data.send_fd = 1;
                        pack.data.recv_fd = 0;
                        send(current_user -> item.user.user_socket,&pack,sizeof(pack),0);
                    }
                    mysql_node * new_chat;
                    new_chat = (mysql_node *)malloc(sizeof(mysql_node));
                    new_chat -> item.chat.send_user = group_account;
                    new_chat -> item.chat.recv_user = accounted;
                    strcpy(new_chat -> item.chat.messages,"您已被提出群聊");
                    new_chat -> item.chat.send_can_look = 1;
                    new_chat -> item.chat.recv_can_look = 1;
                    new_chat -> item.chat.message_type = 1;
                    char nowTime[81] = "\0";
                    getNowTime(nowTime);
                    strcpy(new_chat -> item.chat.date,nowTime);
                    new_chat -> next = NULL;
                    mysql_list(&head_chat,new_chat);
                    current = head_chat;
                    printf("---------------------\n");
                    while(current != NULL)
                    {
                        printf("%d , %s , %s \n",current -> item.chat.send_user,current -> item.chat.messages,current ->item.chat.date);
                        current = current -> next;
                    }
                    printf("-----------------------\n");
                    memset(string,0,sizeof(string));
                    sprintf(string,"insert into chat_messages values(%d,%d,'%s',%d,%d,%d,'%s')",
                    group_account,accounted,new_chat -> item.chat.messages,1,1,1,nowTime);
                    mysql_query(&mysql,string);
                    break;
                }
                current_user = current_user -> next;
            }
            return 0;
        }
    }
    
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
