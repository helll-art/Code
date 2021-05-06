#include "Teacher.h"
#include <fstream>
#include "Appointment.h"
#include "define.h"
#include<string.h>
#include <iomanip>
using namespace std;

void Teacher :: operator = (Teacher & te)
{
    this -> setAccount(te.getAccount());
    this -> setFlag(te.getFlag());
    this -> setPasswd(te.getPasswd());
    this -> setName(te.getName());
}
void Teacher :: Login_Tips()
{
    ifstream ifs;
    ifs.open(APPOINTMENT , ios :: in | ios :: binary );
    if(!ifs.is_open())
    {
        cout << "文件打开失败" <<  __LINE__ << endl;
        exit(0);
    }
    int sum = 0;
    Appointment ap;
    while(ifs.read((char *) & ap, sizeof(Appointment)))
    {
        if(ap.getAskState() == WAIT)
        {
            sum ++;
        }
    }
    ifs.close();
    cout << "\033[1m \033[31m有 " << sum << " 条申请记录待通过， 请及时查看！\033[0m" << endl;

}
void Teacher :: Menu()
{
    cout << "+=================================教师界面================================+" << endl;
    cout << "|           1.查看所有预约                          2.审核预约            |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "|                               0.退出登录                                |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "+=========================================================================+" << endl;
}
void Teacher :: Look_All_Appointment()
{
    Appointment ap;
    ifstream ifs;
    ifs.open(APPOINTMENT , ios :: in | ios :: binary);
    if(!ifs.is_open())
    {
        cout << "文件打开失败 " << __LINE__ << endl;
        exit(0);
    }
    
    time_t t;
    cout << "+------------------------+------------------------+---------------------------+--------------------+-----------+" << endl;
    cout << "|       预约机房号       |        预约时间        |         申请时间          |        学生        |    状态   |"  << endl;
    cout << "+------------------------+------------------------+---------------------------+--------------------+-----------+" << endl;
    while(ifs.read((char *) & ap,sizeof(Appointment)))
    {   
        
        cout << "|             " << ap.getCmpRoom() << "          " ;
        cout << "|   星期： " << ap.getDay() << "  课时 ： " << ap.getTimeInterval() ; 
        cout << "  |  " ;
        t = ap.getTime();
        Display_Time(&t);
        cout << "  |  " << setw(12) << ap.getStudent().getName() << "      |" ;
        if(ap.getAskState() == WAIT)
        {
            cout << "   待审核  |" << endl; 
        }
        else if(ap.getAskState() == PASS)
        {
            cout << "    通过   |" << endl;
        }
        else if(ap.getAskState() == UN_PASS)
        {
            cout << "  被驳回   |" << endl;
        }
        cout << "+------------------------+------------------------+---------------------------+--------------------+-----------+" << endl;
        
        
    }
}
void  Teacher :: Display_Time(time_t *curr)
{
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
void Teacher :: Audit_Appointment()
{
    fstream fs;
    fs.open(APPOINTMENT , ios :: in | ios :: binary | ios :: out );
    if(!fs.is_open())
    {
        cout << "文件打开失败" << endl;
        exit(0);
    }
    Appointment ap;
    time_t t;
    char ch;
    
    while(fs.read((char * ) & ap, sizeof(Appointment)))
    {
        if(ap.getAskState() == WAIT)
        {
            cout << "       预约机房号 ： " << ap.getCmpRoom() ;
        cout << "                  预约时间 ： 星期 ： " << ap.getDay() << "  课时 ： " << ap.getTimeInterval() << endl;
        cout <<  "       学生 ：  " << setw(12) << ap.getStudent().getName() ;
        cout << "            申请时间 ： "  ;
        t = ap.getTime();
        Display_Time(&t);
        cout << endl;
        cout << "是否审核该条信息 ( y / n ) : ";
        cin >> ch;
        getchar();
        if(ch == 'Y' || ch == 'y')
        {
            cout << "是否通过该条申请   ( y / n ) : " ;
            cin >> ch;
            getchar(); 
            if(ch == 'Y' || ch == 'y')
            {

                fs.seekp(-1 * sizeof(Appointment) , ios :: cur);
                ap.setAskState(PASS);
                fs.write((char * ) & ap,sizeof(Appointment));
                cout << "通过成功"  << endl;
                ch = '\0';
            }
            else if(ch == 'N' || ch == 'n')
            {
                fs.seekp(-1 * sizeof(Appointment) , ios :: cur);
                ap.setAskState(UN_PASS);
                fs.write((char * ) & ap,sizeof(Appointment));
                cout << "拒绝成功"  << endl;
                ch = '\0';
            }
            else 
               cout << "未操作该申请" << endl;
            cout << "是否继续审查申请   ( y / n ) : ";
            cin >> ch;
            getchar();
            if(ch == 'Y' || ch == 'y')
                continue;
            else 
                return ;
        }
        else 
        {
            continue;
        }

        }
    }
}
void Teacher :: Exit()
{
    cout << "欢迎下次使用~" << endl;
    exit(0);
}