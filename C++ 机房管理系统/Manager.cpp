#include "Manager.h"
#include "Account.h"
#include<fstream>
#include<string.h>
using namespace std;

void Manager :: Stu_Menu(Student & stu)
{
    int choose = -1;
    while(1)
    {
        stu.Menu();
        cout << "请输入您的选择 : ";
        cin >> choose;
        switch(choose)
        {
            case 1: 
                stu.Ask_Appointment();
                break;
            case 2 :
                stu.Look_Mine_Appointment();
                break;
            case 3 :
                stu.Look_All_Appointment();
                break;
            case 4 :
                stu.Cancel_Appointment();
                break;
            case 0 :
                stu.Exit();
                break;
            default : 
                break;
        }
    }
    
}
void Manager :: Tea_Menu(Teacher & tea)
{
    tea.Login_Tips();
    int choose = -1;
    while(1)
    {
        tea.Menu();
        cout << "请输入您的选择 : ";
        cin >> choose;
        switch(choose)
        {
            case 1: 
                tea.Look_All_Appointment();
                break;
            case 2 :
                tea.Audit_Appointment();
                break;
            case 0 :
                tea.Exit();
                break;
            default : 
                break;
        }
    }
    
}

void Manager :: Adm_Menu(Admin & ad)
{
           
    int choose = -1;
    
    while(1)
    {
        ad.Menu();
        cout << "请输入您的选择 ： " ;
        cin >> choose;
        switch(choose)
        {
            case 1 :
                ad.Add_Account();
                break;
            case 2 :
                ad.Look_Message();
                break;
            case 3 :
                ad.Look_CptRoom();
                break;
            case 4 : 
                ad.Clear_File();
                break;
            case 5 :
                ad.Del_Account();
                break;
            case 6 :
                ad.Update_Account();
                break;
            case 0 :
                ad.Exit();
                break;
            default :
                break;
        }
    }
    

}   
void Manager ::  Menu()
{
    cout << "============================欢迎登录机房管理系统===========================" << endl;
    cout << "+--------------------------选择您的身份并进行登录-------------------------+" << endl;
    cout << "|                                                                         |" << endl;
    cout << "|                                1.学生                                   |" << endl;
    cout << "|                                                                         |" << endl;
    cout << "|                                2.老师                                   |" << endl;
    cout << "|                                                                         |" << endl;
    cout << "|                                3.管理员                                 |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "|                                0.退出                                   |" << endl;
    cout << "|                                                                         |" << endl;
    cout << "+-------------------------------------------------------------------------+" << endl;

}

void Manager :: Display_Time(time_t *curr)
{
    time(curr);
    tm *t;
    t = localtime(curr);
    cout << t -> tm_year + 1900 << "." ;
    cout << t -> tm_mon + 1 << ".";
    cout << t -> tm_mday <<"   ";
    cout << t -> tm_hour << " : ";
    if(t -> tm_min < 10)
        cout << "0" ;
    cout << t -> tm_min  << " : ";
    if(t -> tm_sec < 10)
        cout << "0" ;
    cout << t -> tm_sec  ;
}

void Manager :: Exit()
{
    cout << "欢迎下次使用~ " << endl;
    exit(0);

}
pair<bool,Account> Manager :: Verify_Account(int account , char * passwd)
{
    Account ac;
    pair<bool,Account> a(false,ac);
    ifstream ifs;
    //定位到文件头
    ifs.open("./file/Account.dat", ios :: binary | ios :: in );
    if(!ifs.is_open())
    {
        cout << "文件打开失败" << endl;
        exit(-2);
    }
    while(ifs.read((char * ) &ac,sizeof(Account)))
    {
        
        if(ac.getAccount() == account && (strcmp(ac.getPasswd() , passwd) == 0))
        {
            a.first = true;
            a.second = ac;
            return a;
        }
    }
    
    ifs.close();
    return a;
}