#ifndef _TEACHER_H
#define _TEACHER_H
#include<iostream>
#include "Account.h"

using namespace std;

class Teacher : public Account
{
    public :
        Teacher() {}
        void Menu();
        void Exit();
            //查看所有预约 日期，时段，学号，学生姓名，机房号，预约状态
        void Look_All_Appointment();
            //审核预约    通过，不通过
        void Audit_Appointment();
            //注销登录
        void Teacher_Exit();
            //上线提示
        void Login_Tips();
            //展示时间
        void Display_Time(time_t *curr);
        void operator = (Teacher & te);
    private :
        Account ac;
};
#endif