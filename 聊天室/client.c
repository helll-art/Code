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
#define SIGNAL_SEND 101
#define ADD_FRIEND 103
#define EXIT_ACCOUNT 114


void client_UI(void);
char getch();
int GetPassword(char* passwd) ;
void Account_UI(void);
void my_err(const char *string,int line);
void * recv_fun(void * arg);
void *send_fun(void *arg);


/*
    BUG :
        1.查找密码时，不能正确处理不存在的账户

*/

int ACCOUNT;
BOX box;



int main()
{
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
                    int ret_EA = -1;
                    recv(conn_fd,&ret_EA,sizeof(ret_EA),0);
                    if(ret_EA == 1)
                    {
                        close(conn_fd);
                        printf("Bye!\n");
                        exit(1);
                    }
                    else 
                    {
                        printf("请重新输入：\n");
                        continue;
                    }
                break;
 
                case SIGNAL_SEND:
                

        
                    if(pthread_create(&th2, NULL, send_fun, (void*)&conn_fd)){
                    
                        perror("Create phtread error\n");
                        
                        exit(1);
                    
                    }
                    pthread_join(th2, NULL);
                    break;

                case ADD_FRIEND:
                    pack.data.send_account = ACCOUNT;
                    sprintf(stderr,"请输入要添加的好友账户：");
                    scanf("%d",&pack.data.recv_account);
                    getch();

                    send(conn_fd,&pack,sizeof(pack),0);
                    int ret_AF = -1;
                    recv(conn_fd,&ret_AF,sizeof(ret_AF),0);


                    if(ret_AF == 1)
                    {
                        sprintf(stderr,"对方已是好友，无需重复添加！\n");
                        break;
                    }
                    else if(ret_AF == 2) 
                    {
                        sprintf(stderr,"发送请求成功！\n");
                        break;
                    }
                    else 
                    {
                        sprintf(stderr,"发送请求失败，请重试！\n");
                        break;
                    }
                    


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
            printf("%s",pack.data.write_buf);
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

        printf("是否继续发送(y/n)\n");
        scanf("%c",&c);
        
    } while (c == 'y' || c== 'Y');

    
}


void * recv_fun(void * arg)
{
    PACK pack;
    
    int cfd = *((int *)arg);
    while(1)
    {
        memset(&pack,0,sizeof(pack));
        int n = 0;
        if((n = recv(cfd,&pack,sizeof(pack),0)) > 0)
        {
            printf("叮！您有一条新消息，请注意查收\n");
//1 一般消息        2.好友请求           3.文件
            box.type[box.number] = pack.type; 
            if(box.type[box.number] == 1)
            {
                box.number ++;
                box.talk_number ++;
                box.send_account[box.talk_number] = pack.data.send_account;
                strcpy(box.read_buf[box.talk_number],pack.data.write_buf);
            }
            else if(box.type[box.number] == 2)
            {
                box.number ++;
                box.friend_number ++;
                box.plz_account[box.friend_number] = pack.data.send_account;

            }

/*
             else if(box.type[box.number] == 3)
            {


                文件

            }
*/
        }
        
    }
}


