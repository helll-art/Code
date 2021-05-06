#pragma once
#include<iostream>
#include<time.h>
#include<map>
#include "Appointment.h"
#include "Admin.h"
#include "Teacher.h"
using namespace std;

class Manager
{
    public :
            //学生界面
        void Stu_Menu(Student & stu);
            //老师界面
        void Tea_Menu(Teacher & tea);
            //管理员界面
        void Adm_Menu(Admin & ad);
            //登录界面
        void Menu();
            //显示时间
        void Display_Time(time_t *curr);
            //验证账户密码
        pair<bool,Account> Verify_Account(int Account , char * Passwd);
        void Exit();
    private :
        //维护申请的类链表
        map<time_t , Appointment> mp; 
};