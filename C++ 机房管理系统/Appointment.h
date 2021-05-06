#ifndef _APPOINTMENT_H
#define _APPOINTMENT_H
#include<iostream>
#include "Student.h"
#include "Teacher.h"

//预约的学生编号，正式预约的时间，预约的时段，预约的机房，
using namespace std;

class Appointment
{
    public :
        Appointment() {}
        Appointment(Teacher m_tea,Student m_stu,time_t m_time , int m_day, int m_time_interval,int m_ask_state)
        {
            tea = m_tea;
            stu = m_stu;
            
            Ask_Time = m_time;
            day = m_day;
            time_interval = m_time_interval ;
            ask_state = m_ask_state;

        }
        Student getStudent();
        Teacher getTeacher();
        time_t getTime();
        int getDay();
        int getTimeInterval();
        int getAskState();
        int getCmpRoom();

        void setStudent(Student  &st);
        void setTeacher(Teacher & te);
        void setAskTime(time_t t);
        void setDay(int d);
        void setTimeInterval(int ti);
        void setAskState(int as);
        void setCmpRoom(int cr);
    
    //private : 
            //申请预约的学生
        Student stu;
            //审核的老师
        Teacher tea;
            //申请预约的时间
        time_t Ask_Time ; 
            //预约的时间
        int day;
            //预约的时段
        int time_interval;
            //当前预约的状态
        int ask_state;
        int CmpRoom;
};

#endif