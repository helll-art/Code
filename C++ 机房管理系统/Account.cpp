#include<iostream>
#include"Account.h"
using namespace std;
Account :: Account(char * a,int b,char * pwd,int fg)
{
    for(int i = 0 ; i < sizeof(a) ; ++ i)
    {
        name[i] = a[i];
    }
    for(int i = 0 ; i < sizeof(pwd) ; ++ i)
    {
        passwd[i] = pwd[i]; 
    }
    account = b;
    flag = fg;
}
char * Account :: getName()
{
    return name;
}
int Account :: getAccount()
{
    return account;
}
int Account :: getFlag()
{
    return flag;
}
char * Account :: getPasswd()
{
    return passwd;
}
char * Account :: setName(char * na)
{
    for(int i = 0 ; i < sizeof(na) ; ++ i)
    {
        name[i]  = na[i];
    }
    
    return name;
}
int Account :: setAccount(int acc)
{
    account = acc;
    return account;
}
int Account :: setFlag(int fl)
{
    flag = fl;
    return flag;
}
char * Account :: setPasswd(char * pw)
{
    for(int i = 0 ; i < sizeof(pw) ; ++ i)
    {
        passwd[i]  = pw[i];
    }
    return passwd;
}
void Account :: Exit()
{
    
    cout << "欢迎下次使用~" << endl;
    exit(0);
}