#pragma once
#include<iostream>
#include "Account.h"
using namespace std;


class Admin : public Account
{
    public :
        Admin() {}
        
        void Menu();
            //添加账号
        void Add_Account();
            //查看学生 / 教师信息
        void Look_Message();
            //查看机房     显示机房编号及最大容量
        void Look_CptRoom();
            //清空预约文件
        void Clear_File();

            //删除账户
        void Del_Account();
            //修改账户信息
        void Update_Account();
            
            //读取信息，Look_Message 的辅助函数
        void Read_Message(string path , int flag);

    
        
};