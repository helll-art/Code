#include "Appointment.h"
using namespace std;

void Appointment :: setStudent(Student & st)
{
    this ->stu = st;
    
}
void Appointment :: setTeacher(Teacher & te)
{
    this -> tea = te;
} 
void Appointment :: setAskTime(time_t t)
{
    this -> Ask_Time = t;
}
void Appointment :: setDay(int d)
{
    this -> day = d;
}
void Appointment :: setTimeInterval(int ti)
{
    this -> time_interval = ti;
}
void Appointment :: setAskState(int as)
{
    this -> ask_state = as;
}
void Appointment :: setCmpRoom(int cr)
{
    this -> CmpRoom = cr;
}
Student Appointment :: getStudent()
{
    return this -> stu;
}
Teacher Appointment :: getTeacher()
{
    return this -> tea;
}
time_t Appointment :: getTime()
{
    return this -> Ask_Time;
}
int Appointment :: getDay()
{
    return this ->  day;
}
int Appointment :: getTimeInterval()
{
    return this -> time_interval;
}
int Appointment :: getAskState()
{
    return this -> ask_state;
}
int Appointment :: getCmpRoom()
{
    return this -> CmpRoom;
}