#ifndef _ACCOUNT_H
#define _ACCOUNT_H
#include<iostream>

using namespace std;

class Account
{
    public : 
        Account(char * a,int b,char * pwd,int fg);
        Account() : name() , account(0) , passwd() , flag(-1) {}
        char *  getName();
        int getAccount();
        int getFlag();
        char * getPasswd();
        char * setName(char * na);
        int setAccount(int acc);
        int setFlag(int fl);
        char * setPasswd(char * pw);
        void Exit();
    private : 
        char name[20];
        int account;
            //账号类型
        int flag; 
        char passwd[20];

};
#endif 