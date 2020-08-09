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

#include"my_pack.h"



#define PORT 8888
#define MAX_SIZE 1024
#define EPOLLMAX 1024

#define LOGIN 1
#define REGISTERED 2
#define FIND_PASSWORD 3
#define CHANGE_PASSWORD 33
#define EXIT_CUI 4
#define SIGNAL_SEND 101   //私聊
#define GROUP_CHAT 102 
#define ADD_FRIEND 103    //添加好友
#define DEL_FRIEND 104    //删除好友
#define LOOK_FRIEND 105    //查询好友列表
#define LOOK_CHAT_RECORD 106
#define SET_REALTION 107

#define CREATE_GROUP 108
#define ADD_GROUP 1081
#define RETURN_GROUP 1082
#define FREE_GROUP 109
#define LOOK_GROUP_MEMBER 110
#define LOOK_GROUP 111

#define EXIT_ACCOUNT 114  
#define LOOK_MESSAGEBOX 115
#define MANAGE_FRIEND 116
#define MANAGE_MESSAGE 117
#define MANAGE_GROUP 118


void client_UI(void);
char getch();
int GetPassword(char* passwd) ;
void Account_UI(void);
void my_err(const char *string,int line);
void * recv_fun(void * arg);
void *send_fun(void *arg);
void Cut_Time(char *time,char *string);
int Look_MessageBox(BOX *box);
int Look_Friends(BOX *box);
int Look_Chat_Record(BOX *box);
int Look_Group_Member(void);
int Look_Group(void);

/*
    BUG :
        1.查找密码时，不能正确处理不存在的账户

*/

int ACCOUNT;
BOX box;
int CONN_FD;
int ret ;

pthread_mutex_t mutex;
pthread_cond_t cond;

int main()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    pthread_t th1,th2;
    int conn_fd;
    int serv_port = PORT;
    struct sockaddr_in serv_addr;
    char recv_buf[MAX_SIZE];
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serv_port);
    inet_aton("127.0.0.1",&serv_addr.sin_addr);
    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr));
    CONN_FD = conn_fd;


    while(1)
    {
        PACK pack;
        memset(&pack,0,sizeof(pack));
        client_UI();
        scanf("%d",&pack.type);
        getch();
        if(pack.type >4 && pack.type <0)
        {
            continue;
        }
        else 
        {
            switch(pack.type)
            {
                case  LOGIN:
                    fprintf(stderr,"请输入账户：");
                    scanf("%d",&pack.data.send_account);
                    getch();
                    int ret_LO = -1;                    
                    for(int i = 2;i > 0;i --)
                    {
                        fprintf(stderr,"请输入密码：");
                        GetPassword(pack.data.send_user);
                        
                        if((ret_LO = send(conn_fd,&pack,sizeof(pack),0)) < 0)
                       {
                           my_err("send error",__LINE__);
                           continue;
                       }
                       else 
                       {
                           recv(conn_fd,&ret_LO,sizeof(ret_LO),0);
                           if(ret_LO == 1)
                           {
                               ACCOUNT = pack.data.send_account;
                               printf("密码正确，欢迎来到聊天室！\n");
                               printf("正在登录，请稍候......\n");
                               int i;
                               recv(conn_fd,&i,sizeof(i),0);
                               printf("接收到%d条消息,注意查收！\n",i);
                               sleep(1);
                               goto abc;
                           }
                           else 
                           {
                               fprintf(stderr,"\n密码错误，请重新输入！！！（还有%d次机会）",i);
                           }
                       }
                    }
                    break;
                    
                    
                case REGISTERED:
                    fprintf(stderr,"请输入昵称：");
                    fgets(pack.data.recv_user,sizeof(pack.data.recv_user),stdin);
                    pack.data.recv_user[strlen(pack.data.recv_user) - 1] = '\0';
                    
                    fprintf(stderr,"请输入密码：");
                    GetPassword(pack.data.send_user);
                    fprintf(stderr,"\n请再次输入密码：");
                    char password[50] = "\0";
                    GetPassword(password);
                    int ret_RA = -1;
                    if(strcmp(pack.data.send_user,password) == 0)
                    {
                        fprintf(stderr,"\n请输入密保问题:");
                        fgets(pack.data.read_buf,sizeof(pack.data.read_buf),stdin);
                        pack.data.read_buf[strlen(pack.data.read_buf) - 1]  = '\0';
                        
                        
                        fprintf(stderr,"请输入密保答案：");
                        fgets(pack.data.write_buf,sizeof(pack.data.write_buf),stdin);
                        pack.data.write_buf[strlen(pack.data.write_buf) - 1] ='\0';

                        if((ret_RA = send(conn_fd,&pack,sizeof(pack),0)) < 0)
                        {
                            my_err("send error",__LINE__);
                            break;
                        }
                        else 
                        {
                            ret_RA = -1;
                            recv (conn_fd,&ret_RA,sizeof(ret_RA),0);
                            if(ret_RA > 0)
                            {
                                printf("申请到的账户为%d\n欢迎登录!!!\n",ret_RA);
                            }
                            else 
                            {
                                printf("注册失败！\n");
                                continue;
                            }
                            break;
                        }
                    }
                    else 
                    {
                        memset(password,0,sizeof(password));
                        fprintf(stderr,"\n两次密码不一致，请再次输入:");
                        GetPassword(password);
                        if(strcmp(pack.data.send_user,password) == 0)
                        {
                            fprintf(stderr,"\n请输入密保问题:");
                            fgets(pack.data.read_buf,sizeof(pack.data.read_buf),stdin);
                            pack.data.read_buf[strlen(pack.data.read_buf) - 1]  = '\0';
                            
                            
                            fprintf(stderr,"请输入密保答案：");
                            fgets(pack.data.write_buf,sizeof(pack.data.write_buf),stdin);
                            pack.data.write_buf[strlen(pack.data.write_buf) - 1] = '\0';
                            
                            if((ret_RA = send(conn_fd,&pack,sizeof(pack),0)) < 0)
                            {
                                my_err("send error",__LINE__);
                                break;
                            }
                            else 
                            {
                                ret_RA = -1;
                                recv (conn_fd,&ret_RA,sizeof(ret_RA),0);
                                if(ret_RA > 0)
                                {
                                    printf("申请到的账户为%d\n欢迎登录!!!\n",ret_RA);
                                }
                                else 
                                {
                                    printf("注册失败！\n");
                                    continue;
                                }
                                break;
                            }
                        }
                    }
                    break;

                case  FIND_PASSWORD:
                    fprintf(stderr,"请输入要查找密码的账户：");
                    scanf("%d",&pack.data.send_account);
                    getch();
                    int ret_FP = -1;
                    if((ret_FP = send(conn_fd,&pack,sizeof(pack),0)) < 0)
                    {
                        my_err("find password send error",__LINE__);
                        continue;
                    }

                    ret_FP = recv(conn_fd,&pack,sizeof(pack), 0);
                    if(ret_FP < 0)
                    {
                        my_err("find password error",__LINE__);
                        continue;
                    }
                    
                    char daan[1024] = "\0";
                    fprintf(stderr,"密保问题：%s",pack.data.write_buf);
                    fprintf(stderr,"\n请输入密保答案：");
                    fgets(daan,sizeof(daan),stdin);
                    daan[strlen(daan) - 1] ='\0';
                    
            
                    if(strcmp(pack.data.read_buf,daan) == 0)
                    {
                        ACCOUNT = pack.data.send_account;
                        printf("账户：%d的密码是：%s\n",pack.data.send_account,pack.data.send_user);
                        char c;
                        printf("是否修改密码(y/n)\n");
                        scanf("%c",&c);
                        getch();
                        if(c == 'y')
                        {
                            memset(&pack,0,sizeof(pack));
                            
                            printf("请输入修改后的密码：");
                            GetPassword(pack.data.send_user);
                            pack.data.send_account = ACCOUNT;
                            pack.type = CHANGE_PASSWORD;
                            int ret_CP = -1;
                            if((ret_CP = send(conn_fd,&pack,sizeof(pack),0)) < 0)
                            {
                                my_err("send error",__LINE__);
                                continue;
                            }
                            else 
                            {
                                ret_CP = -1;
                                recv(conn_fd,&ret_CP,sizeof(ret_CP),0);
                                if(ret_CP == -1)
                                {
                                    printf("修改密码失败！\n");
                                    continue;
                                }
                                else 
                                {
                                    printf("密码修改成功，请重新登录！\n");
                                    continue;
                                }
                                break;
                            }
                        }
                        else 
                        {
                            printf("输入密保答案错误！\n");                           
                            continue;
                        }
                    }
                    break;
                case EXIT_CUI:
                    send(conn_fd,&pack,sizeof(pack),0);
                    fprintf(stderr,"Bye!\n");
                    close(conn_fd);
                    return 0;
                break;

                default :
                    printf("请输入正确选项！\n");
                    break;

            }
        }
    }

abc :   
        

        
        if(pthread_create(&th1, NULL, recv_fun, (void*)&conn_fd)){
            printf("recv fun create\n");
            perror("Create phtread error\n");
            
            exit(1);
        
        }
       
        
        
        

        while(1)
        {
            Account_UI();
            PACK pack;

            scanf("%d",&pack.type);
            getch();
            switch(pack.type)
            {
                case EXIT_ACCOUNT:
                
                    pack.data.send_account =ACCOUNT;
                    int n;
                    if((n = send(conn_fd,&pack,sizeof(pack),0)) < 0)
                    {
                        printf("请再次输入:\n");
                       
                        continue;
                    }
                    
                    fprintf(stderr,"正在退出，请稍候...\n");
                    sleep(1);
                    
                    if(ret == 1)
                    {
                        close(conn_fd);
                        printf("Bye!\n");
                        ret = 0;
                        exit(1);
                    }
                    else 
                    {
                        printf("请重新输入：\n");
                        ret = 0;
                        continue;
                    }
                
                break;
 
                case SIGNAL_SEND:
                

                    
                    if(pthread_create(&th2, NULL, send_fun, (void*)&conn_fd)){
                    
                        perror("Create phtread error\n");
                        
                        exit(1);
                    
                    }
                    
                    pthread_join(th2, NULL);
                    
                    sleep(1);
                    printf("正在处理...\n");
                    if(ret == -1)
                    {
                        printf("未添加对方为好友！\n");
                        ret = 0;
                    }
                    ret = 0;
                    
                    break;
                case GROUP_CHAT:

                    fprintf(stderr,"请输入群聊账户：");
                    scanf("%d",&pack.data.send_account);
                    getch();
                    fprintf(stderr,"请输入要发送的内容：");
                    fgets(pack.data.write_buf,sizeof(pack.data.write_buf),stdin);
                    pack.data.write_buf[strlen(pack.data.write_buf) - 1] = '\0';
                    sprintf(pack.data.write_buf,"%s[账户：%d]\0",pack.data.write_buf,ACCOUNT);
                    printf("%s\n",pack.data.write_buf);
                    pack.data.recv_account = ACCOUNT;
                    send(conn_fd,&pack,sizeof(pack),0);
                    printf("正在处理！\n");
                    sleep(1);
                    if(ret == 1)
                    {
                        printf("发送成功...\n");
                    }
                    else 
                        printf("发送失败...\n");
                    
                    break;
                case ADD_FRIEND:
                
                    pack.data.send_account = ACCOUNT;
                    fprintf(stderr,"请输入要添加的好友账户：");
                    scanf("%d",&pack.data.recv_account);
                    getch();
                    pack.data.send_fd = 3;
                    
                    ret = 0;
                    send(conn_fd,&pack,sizeof(pack),0);
                    printf("正在处理，请稍候...\n");
                    sleep(1);
                    
                    if(ret == -1)
                        fprintf(stderr,"该账户不存在，请重新输入...\n");
                    else if(ret == 1) 
                        fprintf(stderr,"发送请求成功...\n");
                    else if(ret == 0)
                        fprintf(stderr,"已添加对方为好友,请勿重复添加...\n");
                    else 
                        fprintf(stderr,"发送好友请求失败...\n");
                
                    
                break;
                    case DEL_FRIEND:
                    
                        pack.data.send_account = ACCOUNT;
                        fprintf(stderr,"请输入要删除的好友账户：");
                        scanf("%d",&pack.data.recv_account);
                        getch();
                      
                        send(conn_fd,&pack,sizeof(pack),0);
                        
                        printf("正在处理，请稍后...\n");
                        sleep(1);
                        
                        if(ret == 1)
                            fprintf(stderr,"删除成功...\n");
                        else if(ret == 0)
                            printf("输入的账户不存在,请再次确认...\n");
                        else if(ret == -1)
                            printf("在好友列表中未找到该用户...\n");
                        else if(ret == -2)
                            printf("删除失败...\n");
                        ret = 0;
                    break;
                    case LOOK_FRIEND:

                        pack.data.send_account = ACCOUNT;
                        send(conn_fd,&pack,sizeof(pack),0);
                        Look_Friends(&box);
                        memset(&box,0,sizeof(box));
                    break;
                    case LOOK_CHAT_RECORD:
                        pack.data.send_account = ACCOUNT;
                        send(conn_fd,&pack,sizeof(pack),0);
                        printf("正在处理...\n");
                        sleep(3);
                        Look_Chat_Record(&box);
                        ret = 0;
                    break;
                    case SET_REALTION:
                        
                        fprintf(stderr,"请输入要设置状态的好友账户：");
                        scanf("%d",&pack.data.recv_account);
                        getch();
                        printf("0.黑名单   1.一般好友   2.特别关心   \n");
                        fprintf(stderr,"请输入你要设置的状态:");
                        scanf("%d",&pack.data.send_fd);
                        pack.data.recv_fd = 55;
                        pack.data.send_account = ACCOUNT;
                        send(conn_fd,&pack,sizeof(pack),0);
                        sleep(1);
                        if(ret == 1)
                        {
                            printf("设置状态成功...\n");
                        }
                        else if(ret == -1)
                            printf("不是好友关系...\n");
                        else if(ret == 0)
                            printf("设置失败...\n");
                        ret = 0;
                        break;

                    case CREATE_GROUP:
                        fprintf(stderr,"请输入要创建的群聊名称:");
                        fgets(pack.data.send_user,sizeof(pack.data.send_user),stdin);
                        pack.data.send_user[strlen(pack.data.send_user) - 1] = '\0';
                        
                        pack.data.send_account = ACCOUNT;
                        
                        send(conn_fd,&pack,sizeof(pack),0);
                        
                        sleep(1);
                        if(ret != 0)
                        {
                            printf("创建的群号是：%d\n",ret);
                        }
                        else 
                        {
                            printf("创建群聊失败...\n");
                        }
                        break;

                    case FREE_GROUP:
                        fprintf(stderr,"请输入要解散的群号：");
                        scanf("%d",&pack.data.send_account);
                        pack.data.recv_account = ACCOUNT;
                        send(conn_fd,&pack,sizeof(pack),0);
                        sleep(1);

                        if(ret == 1)
                            printf("解散群聊成功...\n");
                        else 
                            printf("无权解散群聊...\n");

                        break;

                    case LOOK_GROUP_MEMBER:
                        fprintf(stderr,"请输入要查看的群:");
                        scanf("%d",&pack.data.send_account);
                        pack.data.recv_account = ACCOUNT;
                        send(conn_fd,&pack,sizeof(pack),0);
                        printf("正在处理...\n");
                        sleep(3);
                        if(ret == -1)
                        {
                            printf("无权查看该群成员...\n");
                            break;
                        }
                        Look_Group_Member();
                        memset(&box,0,sizeof(box));
                        break;
                    case LOOK_GROUP:
                        pack.data.send_account = ACCOUNT;
                        
                        send(conn_fd,&pack,sizeof(pack),0);
                        printf("正在处理...\n");
                        sleep(3);
                        Look_Group();
                        
                        
                        break;
                    case ADD_GROUP:
                        pack.data.recv_account = ACCOUNT;
                        fprintf(stderr,"请输入要加入的群：");
                        scanf("%d",&pack.data.send_account);
                        send(conn_fd,&pack,sizeof(pack),0);

                        sleep(1);
                        if(ret == -1)
                            printf("输入的群不存在...\n");
                        else if(ret == 1)
                            printf("已加入该群，无需重复添加...\n");
                        else if(ret == 2)
                            printf("请等待群主同意...\n");
                        break;

                    case RETURN_GROUP:
                        pack.data.recv_account = ACCOUNT;
                        fprintf(stderr,"请输入你要退出的群：");
                        scanf("%d",&pack.data.send_account);
                        send(conn_fd,&pack,sizeof(pack),0);
                        printf("正在处理\n");
                        sleep(1);
                        if(ret == 2)
                        {
                            printf("您是群主，不能退出只能解散群...\n");
                        }
                        else 
                            printf("已退出该群...\n");

                        break;

                        
                    case LOOK_MESSAGEBOX:
                        ret = 0;                    
                        pack.data.send_account = ACCOUNT;
                        send(conn_fd,&pack,sizeof(pack),0);
                        printf("正在处理...\n");
                        sleep(3);
                        Look_MessageBox(&box);
                        ret = 0;
                        break;
                default:
                    break;
            }
            

        }

        pthread_join(th1, NULL);
}


void Account_UI(void)
{
    printf("\t\t\t\t\t\t+=====================================================+\n");
	printf("\t\t\t\t\t\t||                   \033[1m\033[34m欢迎来到聊天室\033[0m                  ||\n");
	printf("\t\t\t\t\t\t+=====================================================+\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
    printf("\t\t\t\t\t\t|                    \033[1m\033[37m请输入正确选项\033[0m                   |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[36m101.私聊\033[0m                          |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[36m102.群聊\033[0m                          |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[32m103.添加好友\033[0m                      |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[32m104.删除好友\033[0m                      |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[32m105.查询好友列表\033[0m                  |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[32m106.查询聊天记录\033[0m                  |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[32m107.设置好友状态\033[0m                  |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[33m108.创建群聊\033[0m                      |\n");
    printf("\t\t\t\t\t\t|                    \033[1m\033[33m1081.加入群聊\033[0m                     |\n");
    printf("\t\t\t\t\t\t|                    \033[1m\033[33m1082.退出群聊\033[0m                     |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[33m109.解散群聊\033[0m                      |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[33m110.查看群成员\033[0m                   |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[33m111.查看已加群\033[0m                   |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[33m112.查看聊天记录\033[0m                 |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[35m113.给好友发送文件\033[0m               |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[35m114.退出聊天室\033[0m                   |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t+-------------------------+                           |\n");
	printf("\t\t\t\t\t\t|     \033[1m\033[31m115.查看消息盒子\033[0m    |                          |\n");
	printf("\t\t\t\t\t\t+=========================+===========================+\n");
	printf("请输入正确选项：");
}
void client_UI(void)
{
    printf("\t\t\t\t\t\t=======================================================\n");
	printf("\t\t\t\t\t\t||                   \033[1m\033[40;34m欢迎来到聊天室\033[0m                  ||\n");
	printf("\t\t\t\t\t\t=======================================================\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
    printf("\t\t\t\t\t\t|                    \033[1m\033[34m请输入正确选项\033[0m                   |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[34m1.登录账户\033[0m                       |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[34m2.注册账户\033[0m                       |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[34m3.修改/找回密码\033[0m                  |\n");
	printf("\t\t\t\t\t\t|                    \033[1m\033[34m4.退出本聊天室\033[0m                   |\n");
	printf("\t\t\t\t\t\t|                                                     |\n");
	printf("\t\t\t\t\t\t=======================================================\n");
	printf("请输入正确选项：");
}

char getch()
{
	char c;
	system("stty -echo");  //不回显
	system("stty -icanon");//设置一次性读完操作，如使用getchar()读操作，不需要按enter
	c=getchar();
	system("stty icanon");//取消上面的设置
	system("stty echo");//回显
	return c;
}

int GetPassword(char* passwd) 
{ 
   int c;  
    int n = 0; 
    do
    { 
        c=getch(); 
        if (c != '\n'&& c!='\r' && c!=127)
        { 
            passwd[n] = c; 
            printf("*");
            n++;                   
        } 
        else if ((c != '\n'|c!='\r')&&c==127)//判断是否是回车或者是退格           
        {                           
            if(n>0)
            {
                n--;
                printf("\b \b");//输出退格                                                                      
            }
               
        } 
           
    }while(c != '\n' && c !='\r' ); 
    passwd[n] = '\0';  //消除一个多余的回车
    return n; 
}
void my_err(const char *string,int line)
{
    fprintf(stderr,"%d : ",line);
    perror(string);
}

void *send_fun(void *arg)
{
    
    PACK  pack;
    memset(&pack,0,sizeof(pack));
    int cfd = *((int *)arg);
    
    pack.data.send_account = ACCOUNT;
    fprintf(stderr,"请输入接受消息的账户：");
    scanf("%d",&pack.data.recv_account);
    
    char c='\0';
    do
    {
        ret = 0;
        getch();
        fprintf(stderr,"请输入将要发送的消息类型：(1.一般消息\t2.文件)\n");
        int choose = 0;
        scanf("%d",&choose);
        getch();
        if(choose == 1)
        {
            pack.data.send_fd = 1;

            pack.type = SIGNAL_SEND;
            printf("请输入消息：");
            fgets(pack.data.write_buf,sizeof(pack.data.write_buf),stdin);
            pack.data.write_buf[strlen(pack.data.write_buf) - 1] = '\0';
            
            int n;
            if((n = send(cfd,&pack,sizeof(pack),0)) < 0)
            {
                fprintf(stderr,"发送失败!请重新发送\n");

            }
        
            
        }

        else if(choose == 2)
        {
            pack.data.send_fd = 2;
            /*
                发送文件

            */
        }
    printf("正在处理...\n");
    sleep(1);
    
    if(ret == -1)
    {
        printf("未添加对方为好友！\n");
    }
        printf("是否继续发送(y/n)\n");
        scanf("%c",&c);
        
    } while (c == 'y' || c== 'Y');
    ret = 1;
}


void * recv_fun(void * arg)
{
    PACK pack;
    
    int cfd = *((int *)arg);

    while(1)
    {
        int n = 0;
        if((n = recv(cfd,&pack,sizeof(pack),0)) > 0)
        {
            if(pack.data.recv_fd == 55)
            {   
                ret = pack.data.ret_AAA;
                
                    continue;
            }
            else if(pack.data.recv_fd == 66)
            {
                sleep(1);
                recv(cfd,&box,sizeof(box),0);
                continue;
            }
            
            
             if(pack.data.send_fd == 1)

                fprintf(stderr,"叮！您有一条新消息，请注意查收\n");
//1 一般消息        2.wenjian    3.haoyouqingqiu
            else if(pack.data.send_fd == 1 && pack.data.recv_fd == 2) 
                fprintf(stderr,"\033[1m\033[31m叮！您的特别关心给你发送消息，快快查看\033[0m\n");
            else if(pack.data.send_fd == 2 && pack.data.recv_fd == 2)
                fprintf(stderr,"\033[1m\033[31m叮！您的特别关心给您发来一个文件，快快查看\033[0m\n");
            else if(pack.data.send_fd == 3)
                fprintf(stderr,"您收到一条好友请求，请及时查看\n");
            else if(pack.data.send_fd == 2)
                fprintf(stderr,"您收到一个文件，请注意接收！\n");
             else if(pack.data.send_fd == 4)
                fprintf(stderr,"收到一条群聊请求，请注意查收！\n");
           

 /*           box.type[box.number] = pack.data.send_fd;
            if(box.type[box.number] == 1)      // 消息
            {
                box.number ++;                 //消息总数+1
                box.talk_number ++;            //消息数+1
                box.send_account[box.talk_number] = pack.data.send_account;    //发送者账户
                strcpy(box.read_buf[box.talk_number],pack.data.write_buf);     //发送的消息
            }
            else if(box.type[box.number] == 3)     //好友请求
            {
                box.number ++;                //消息总数+1
                box.friend_number ++;         //好友请求数+1
                box.plz_account[box.friend_number] = pack.data.send_account;   //好友请求的账户

            }
*/
/*
             else if(box.type[box.number] == 3)
            {


                文件

            }
*/
        }
    
    }
}


int Look_MessageBox(BOX *box)
{
    int choose1;
    char c;
    char string[50] = "\0";
    
    fprintf(stderr,"1.消息，2.文件，3.好友请求,4.群聊请求...请输入要查看的选项：");
    scanf("%d",&choose1);
    
    
    getch();
    if(choose1 == 1)
    {
        
        for(int i = 1;i <= box->talk_number;i++)
        {
            
            memset(string,0,sizeof(string));
            Cut_Time(box->message_date[i],string);
            printf("%d%70s\n",box->message_account[i],box->message_date[i]);
            printf("*******************************************************************************************\n");
            printf("message:\n%s\n",box->messages[i]);
            printf("end\n");
            printf("*******************************************************************************************\n\n\n");

        }
        PACK pack1;
        pack1.type = MANAGE_MESSAGE;
        pack1.data.recv_account = ACCOUNT;
     
        send(CONN_FD,&pack1,sizeof(pack1),0);
    }
    else if(choose1 == 3)
    {
        
        for(int i = 1;i <= box->friend_number;i++)
        {
            PACK pack;
            memset(string,0,sizeof(string));
            
            Cut_Time(box->message_date[i],string);
            printf("%d%70s\n",box->plz_account[i],box->plz_date[i]);
            printf("------------------------------------------------------------------------------------\n");
            printf("%s\n",box->read_buf[i]);
            fprintf(stderr,"是否同意好友请求(y/n)");
            scanf("%c",&c);
            if(c == 'Y' || c == 'y')
                pack.data.recv_fd = 1;
            
            
            pack.data.send_account = box->plz_account[i];
            pack.type = MANAGE_FRIEND;
            pack.data.recv_account = ACCOUNT;
            
            send(CONN_FD,&pack,sizeof(pack),0);
            
            sleep(1);
            if(ret == 1)
            {
                printf("添加好友成功，快去聊天吧\n");
            }
            else 
            {
                printf("已拒绝好友请求，添加失败...\n");
            }
            
        }
    }
    else if(choose1 == 4)
    {
        for(int i = 1;i <= box->plz_number;i++)
        {
            PACK pack;
            memset(string,0,sizeof(string));
            Cut_Time(box->message_date[i],string);
            printf("%d%70s\n",box->plz_group[i],box->plz_date[i]);
            
            printf("------------------------------------------------------------------------------------\n");
            printf("%s\n",box->write_buf[i]);
            fprintf(stderr,"是否同意群聊请求(y/n)");
            scanf("%c",&c);
            if(c == 'Y' || c == 'y')
               pack.data.recv_fd = 1;
            
            pack.data.send_account = box->plz_group[i];
            pack.type = MANAGE_GROUP;
            pack.data.recv_account = ACCOUNT;
            
            send(CONN_FD,&pack,sizeof(pack),0);
            
            sleep(1);
            if(ret == 1)
            {
                printf("添加用户进群成功...\n");
            }
            else 
            {
                printf("添加失败...\n");
            }
            
        
        }
    }
  

}

void Cut_Time(char *time,char *string)
{
    int k = 0,i = 0;
    for(int j = 0;j < 4;j++)
    {
        string[k] = time[i];
        k++;
        i++;
    }
    
    string[k] = '/';
    k++;
    for(int j = 0;j < 2;j ++)
    {
        string[k] =time[i];
        k++;
        i++;
    }
        
    string[k] = '/';
    k++;
    for(int j = 0;j < 2;j++)
    {
        string[k] = time[i];
        k++;
        i++;
    }
    string[k] = '\t';
    k++;
    
    for(int j = 0;j < 2;j++)
    {
        string[k] = time[i];
        k++;
        i++;
    }  
    string[k] = ':';
    k++;
    for(int j = 0; j < 2;j++)
    {
        string[k] = time[i];
        k++;
        i++;
    }
    string[k] = '\0';
    strcpy(time,string);
}

int Look_Friends(BOX *box)
{
    printf("好友编号   好友账户   好友状态\n");
    for(int i = 1;i < box->number + 1;i++)
    {
        if(box->plz_account[i] == 2)
        {
            printf("\033[1m\033[31m%4d%12d\033[0m\t",i,box->message_account[i]);
        }
        else 
        {
            printf("%4d%12d\t",i,box->message_account[i]);
        }
        if(box->type[i] == 1)
        {
            printf("\033[1m\033[31m在线\033[0m\n");
        }
        else 
        {
            printf("离线\n");
        }
    }

}
int Look_Chat_Record(BOX *box)
{
    int choose1;
    char c;
    char string[50] = "\0";
    
    fprintf(stderr,"1.消息，2.文件，3.好友请求...请输入要查看的选项：");
    scanf("%d",&choose1);
    
    
    getch();
    if(choose1 == 1)
    {
        
        for(int i = 1;i <= box->talk_number;i++)
        {
            
            memset(string,0,sizeof(string));
            Cut_Time(box->message_date[i],string);
            printf("%d%70s\n",box->message_account[i],box->message_date[i]);
            printf("*******************************************************************************************\n");
            printf("message:\n%s\n",box->messages[i]);
            printf("end\n");
            printf("*******************************************************************************************\n\n\n");
            
            
        }
        
    }
    else if(choose1 == 3)
    {
        
        for(int i = 1;i <= box->friend_number;i++)
        {
            PACK pack;
            memset(string,0,sizeof(string));
            printf("%d%70s\n",box->plz_account[i],box->plz_date[i]);
            printf("---------------------------------好友申请-------------------------------------\n");
            
        }
    }

}

int Look_Group_Member(void)
{
    printf("%s(%d)\n",box.message_date[0],box.message_account[0]);
    printf("---------------------------------------------\n");
    printf("账户                昵称                 群地位\n");
    for(int i = 1;i <=box.number ;i++)
    {
        printf("%-20d%-20s",box.message_account[i],box.message_date[i]);
        if(box.type[i] == 1)
            printf("\033[1m\033[31m群主\033[0m\n");
        else if(box.type[i] == 2)
            printf("\033[1m\033[34m管理员\033[0m\n");
        else if(box.type[i] == 3)
            printf("群成员\n");
    }
}

int Look_Group(void)
{
    printf("群账户              群名                群地位\n");
    printf("---------------------------------------------\n");

    for(int i = 1;i <= box.number;i++)
    {
        fprintf(stderr,"%-20d%-20s",box.message_account[i],box.message_date[i]);
        if(box.type[i] == 1)
        {
            fprintf(stderr,"      \033[1m\033[31m群主\033[0m\n");
        }
        else if(box.type[i] == 2)
        {
            fprintf(stderr,"      \033[1m\033[34m管理员\033[0m\n");
        }
        else if(box.type[i] == 3)
        {
           fprintf(stderr,"      群成员\n");
        }
    }
    return 1;
}