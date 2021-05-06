#include "Student.h"
#include "Appointment.h"
#include "define.h"
#include "string.h"
#include<fstream>
#include <iomanip>
using namespace std;

void Student :: Ask_Appointment()
{
a :     int lesson = -1;
    int date = -1; 
    int choose = -1;
    Appointment ap;
    
    cout << "请选择要预约的机房 ： " << endl;
    cout << "1. 机房1 - 最大容纳30人" << endl;
    cout << "2. 机房2 - 最大容纳50人" << endl;
    cout << "3. 机房3 - 最大容纳70人" << endl;
    cin >> choose;
    getchar();
    cout << "请选择预约日期 : " << endl;
    cout << " 1.周一    2.周二" << endl;
    cout << " 3.周三    4.周四" << endl;
    cout << "     5.周五" << endl;
    cin >> date;
    getchar();
    cout << "请选择预约时段 ： " << endl;
    cout << " 1. 1-2节课     2. 3-4节课" << endl;
    cout << " 3. 5-6节课     4. 7-8节课" << endl;
    cin >> lesson;
    getchar();
    if(!( 0 < choose < 4 && 0 < date < 6 && 0 < lesson < 5 ))
    {
        cout << "输入信息有误" << endl;
        goto  a; 
    }
    if(!If_Appointment(choose,date,lesson))
    {
        cout << "该时段已经被预约，请等待老师审核" << endl;
        return ;
    }
    ap.setStudent(*this);
    ap.setCmpRoom(choose);
    ap.setDay(date);
    ap.setTimeInterval(lesson);
    ap.setAskState(WAIT);
    time_t  curr; 
    time(&curr);
    ap.setAskTime(curr);
    ofstream ofs;
    ofs.open(APPOINTMENT , ios :: out | ios :: binary | ios :: app);
    ofs.write((char *) &ap , sizeof(Appointment) );
    ofs.close();
    
}
            //查看我的预约     记录信息，日期，时段，机房号，预约状态
bool Student :: Look_Mine_Appointment()
{
    bool if_Appointment = false;
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
        if(strcmp(ap.getStudent().getName() ,this->getName()) == 0)
        {
            if_Appointment = true;  
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
    return if_Appointment;
}
            //查看所有预约       预约信息，日期，时段，学号，学生姓名，机房好，预约状态
void Student :: Look_All_Appointment()
{
    ifstream ifs;
    ifs.open(APPOINTMENT , ios :: in | ios :: binary);
    if(!ifs.is_open())
    {
        cout << "文件打开失败 " << __LINE__ << endl;
        exit(0);
    }
    time_t t;
    Appointment ap;
    cout << "+------------------------+------------------------+---------------------------+--------------------+-----------+" << endl;
    cout << "|       预约机房号       |        预约时间        |         申请时间          |        学生        |    状态   |"  << endl;
    cout << "+------------------------+------------------------+---------------------------+--------------------+-----------+" << endl;
    while(ifs.read((char *) & ap,sizeof(Appointment)))
    {
        cout << "|           " << ap.getCmpRoom() << "            " ;
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
    }
    cout << "+------------------------+------------------------+---------------------------+--------------------+-----------+" << endl;

}
    //取消预约          取消预约或审核中的预约
void Student :: Cancel_Appointment()
{
    int choose;
    int day;
    int lesson;
    if(false == Look_Mine_Appointment())
    {
        cout << "您暂时没有可以取消的预约 " << endl  ;
        return ;
    }
    cout << "请选择要取消预约的机房号 ：" << endl;
    cout << "1. 机房1 - 最大容纳30人" << endl;
    cout << "2. 机房2 - 最大容纳50人" << endl;
    cout << "3. 机房3 - 最大容纳70人" << endl;
    
    cin >> choose;
    getchar();
    cout << "请选择要取消预约的日期 : " << endl;
    cout << " 1.周一    2.周二" << endl;
    cout << " 3.周三    4.周四" << endl;
    cout << "     5.周五" << endl;
    cin >> day;
    getchar();
    cout << "请选择要取消预约的时段 : " << endl;
    cout << " 1. 1-2节课     2. 3-4节课" << endl;
    cout << " 3. 5-6节课     4. 7-8节课" << endl;
    cin >> lesson; 
    getchar();
    Appointment ap;
    ofstream ofs ;
    ofs.open(APPOINTMENT_TEMP , ios :: out | ios :: binary);
    ifstream ifs;
    ifs.open(APPOINTMENT , ios :: in | ios :: binary);
    bool Cancel_Appointment = false;
    while(ifs.read((char *) & ap , sizeof(Appointment)))
    {
        if(ap.getCmpRoom() == choose && ap.getDay() == day && ap.getTimeInterval() == lesson && (0 == strcmp(ap.getStudent().getName(),this->getName())))
        {
            cout << "取消预约成功" << endl;
            Cancel_Appointment = true;
            continue;
        }
        ofs.write((char *) & ap,sizeof(Appointment));
    }

    ofs.close();
    ifs.close();

    remove(APPOINTMENT);
    rename(APPOINTMENT_TEMP,APPOINTMENT);
    if(Cancel_Appointment == false)
    {
        cout << "取消预约失败 , 请再次尝试" << endl;

    }
}
    //注销登录
void Student :: Student_Exit()
{
    cout << "欢迎下次使用~" << endl;
    exit(0);
}
void Student :: Menu()
{
    cout << "+=================================学生界面================================+" << endl;
    cout << "|           1.申请预约                          2.查看我的预约            |" << endl;
    cout << "|                                                                         |" << endl; 
    cout << "|           3.查看所有预约                      4.取消预约                |" << endl;
    cout << "|                               0.退出登录                                |" << endl;
    cout << "+=========================================================================+" << endl;
}
void Student :: operator = (Student & stu)
{
    this -> setAccount(stu.getAccount());
    this -> setFlag(stu.getFlag());
    this -> setPasswd(stu.getPasswd());
    this -> setName(stu.getName());
}
//机房  日期  时段 
bool Student :: If_Appointment(int choose , int date , int lesson)
{
    ifstream ifs;
    ifs.open(APPOINTMENT, ios :: in | ios :: binary);
    if(!ifs.is_open())
    {
        cout << "文件打开失败" << __LINE__ << endl;
        exit(0);
    }
    Appointment ap;
    while(ifs.read((char * ) &ap , sizeof(Appointment)))
    {
        if(ap.getCmpRoom() == choose && ap.getDay() == date && ap.getTimeInterval() == lesson )
        {
            return false;
        }
    }
    return true;

}   
void  Student :: Display_Time(time_t *curr)
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