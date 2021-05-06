#ifndef _STUDENT_H
#define _STUDENT_H
#include<iostream>
#include "Account.h"

using namespace std;

class Student : public Account
{
    public :
        Student() {} 
        /* Student(Account st) : ac(st) {} */
        
        
            //申请预约      日期，时间，机房，生成记录
        void Ask_Appointment();
            //查看我的预约     记录信息，日期，时段，机房号，预约状态
        bool Look_Mine_Appointment();
            //查看所有预约       预约信息，日期，时段，学号，学生姓名，机房好，预约状态
        void Look_All_Appointment();
            //取消预约          取消预约或审核中的预约
        void Cancel_Appointment();
            //注销登录
        void Student_Exit();
        void Menu();
        void operator = (Student & stu);
            //机房可预约的时间
        void Cmp_Time();
            //是否可以预约
        bool If_Appointment(int choose , int date , int lesson);
        void Display_Time(time_t *curr);
    private :
        Account ac;

};
#endif