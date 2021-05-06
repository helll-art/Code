#include "Admin.h"
#include "Account.h"
#include "define.h"
#include<fstream>
using namespace std;

#define PATH "./file/Account.dat"
#define TEMP_PATH "./file/Account_temp.dat"
void Admin :: Menu()
{
    cout << "+=================================管理员界面==============================+" << endl;
    cout << "|           1.添加账号                                2.查看账号          |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "|           3.查看机房                                4.清空预约          |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "|           5.删除账户                                6.修改账户          |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "|                                 0.退出登录                              |" << endl;
    cout << "+=========================================================================+" << endl;
}
//添加账号
void Admin :: Add_Account()
{
    ofstream ofs;
    
    int i = 0;
    int account ;
    char  name[20];
    int flag;
    char  passwd[20];
    Account ac; 
    int count = 0;
    cout << "请输入要添加账户的个数 : ";
    cin >> count;
    getchar();
    
    ofs.open("./file/Account.dat",ios :: out | ios :: binary | ios :: app);
    while(i < count)
    {
        cout << "<<< 要添加的第 " << i + 1 << " 个账户 >>> " << endl;
        i ++;
        cout << "请输入账户 ： " ;
        cin >> account;
        getchar();
        ac.setAccount(account);
        cout << "请输入密码 ： ";
        cin >> passwd;
        getchar();
        ac.setPasswd(passwd);
        cout << "请选择身份 ：";
        cout << endl << "1.学生      2.老师       3.管理员" << endl;
        cin >> flag;
        getchar();
        ac.setFlag(flag);
        cout << "请输入姓名 ： " ;
        cin >> name;
        getchar();
        ac.setName(name);
        ofs.write((char *) &ac,sizeof(Account));
    }
    ofs.close();
}
    //删除账户
void Admin :: Del_Account()
{
    
    int account;
    cout << "请输入要删除的账户 : " ;
    cin >> account;
    ofstream ofs;
    ofs.open(TEMP_PATH,ios :: out | ios :: binary);
    
    ifstream ifs;
    ifs.open(PATH,ios :: in | ios :: binary);
    if(!ifs.is_open())
    {
        cout << __LINE__ << "文件打开失败" << endl;
        exit(0);
    }
    Account ac;
    while(ifs.read((char *)&ac,sizeof(Account)))
    {
        if(ac.getAccount() == account)
        {
            continue;
        }
        ofs.write((char *)&ac,sizeof(Account));
    }
    ofs.close();
    ifs.close();
    remove(PATH);
    rename(TEMP_PATH , PATH);
    
    
}
    //修改账户信息
void Admin :: Update_Account()
{
    int account;
    cout << "请输入要修改的账户 ：" ; 
    cin >> account;
    Account ac;
    char name[20];
    char passwd[20];
    cout << "[ "<< account << " ]" << "请输入修改后账户的姓名 ： " ;
    cin >> name ;
    cout << "[ "<< account << " ]" << "请输入修改后账户的密码 ： " ;
    cin >> passwd;
    ifstream ifs;
    ofstream ofs;
    ifs.open(PATH,ios :: in | ios :: binary);
    ofs.open(TEMP_PATH , ios :: out | ios :: binary);
    if(!ifs.is_open())
    {
        cout << "文件打开失败  " << endl;
        exit(0);
    }
    while(ifs.read((char *) & ac,sizeof(Account)))
    {
        if(ac.getAccount() == account)
        {
            ac.setPasswd(passwd);
            ac.setName(name);
        }
        ofs.write((char *) &ac , sizeof(Account));
    }
    ifs.close();
    ofs.close();
    remove(PATH);
    rename(TEMP_PATH , PATH);
}
//查看学生 / 教师信息
void Admin :: Look_Message()
{
    cout << "+==========================================================+" << endl;
    cout << "|                         学生账户如下                     | " << endl;
    cout << "+==========================================================+" << endl;
    Read_Message(PATH,STUDENT);
    cout << "+==========================================================+" << endl;
    cout << "|                         老师账户如下                     | " << endl;
    cout << "+==========================================================+" << endl;
    Read_Message(PATH,TEACHER);
    
    cout << "+==========================================================+" << endl;
    cout << "|                         管理员账户如下                   | " << endl;
    cout << "+==========================================================+" << endl;
    Read_Message(PATH,ADMIN);
    
}
//查看机房     显示机房编号及最大容量
void Admin :: Look_CptRoom()
{
    cout << "+------------------+-机房信息暂时不能修改-+-------------------+" << endl;
    cout << "|      房号        |       大小           |       开放时间    |" << endl;
    cout << "+------------------+----------------------+-------------------+" << endl;
    cout << "|     机房1        |        20            |     周一 ~ 周五   |"   << endl;
    cout << "|     机房2        |        50            |     周一 ~ 周五   |"   << endl;
    cout << "|     机房3        |        70            |     周一 ~ 周五   |"   << endl;
    cout << "+------------------+----------------------+-------------------+" << endl;
}
//清空预约文件
void Admin :: Clear_File()
{
    ofstream ofs;
    ofs.open(APPOINTMENT , ios :: out | ios :: trunc);
    ofs.close(); 
    cout << "预约文件清空成功" << endl;
}


void Admin :: Read_Message(string path , int flag)
{
    Account ac;
    ifstream ifs;
    ifs.open(path,ios :: in | ios :: binary);
    if(!ifs.is_open())
    {
        cout << "文件打开失败 " << endl;
        exit(-1);
    }
    while(ifs.read((char *)&ac,sizeof(Account)))
    {
        if(ac.getFlag()  ==  flag)
        {
            cout << "账户 ： " << ac.getAccount() << "    姓名 ： " << ac.getName() << "    密码 ： " << ac.getPasswd() << endl;
        }
    }
    ifs.close();
}