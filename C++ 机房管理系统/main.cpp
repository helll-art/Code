#include<fstream>
#include<iostream>
#include<vector>
#include "define.h"
//#include<ctime>  
#include "Manager.h"

#include "Account.h"
#include "Student.h"
#include "Teacher.h"
#include "Admin.h"
using namespace std;


///string是对象，不是一段内存，不能简单地将其内存保存到文件或从文件中读到内存。



void Getpasswd(char * passwd);
char getch();


int main(int argc, char const *argv[])
{  

    Manager mg;
    
    pair<bool,Account> a;
    int account = 0;
    int choose;
    char passwd[20];
    int count = 0;
    do
    {
        cout << "+=============================================+" << endl;
        cout << "|               欢迎使用机房预约系统          |" << endl;
        cout << "+=============================================+" << endl;
        cout <<"请输入您的账户 : ";
        cin >> account ;
        getchar();
        cout << "请输入您的密码 : ";
        Getpasswd(passwd);
        a = mg.Verify_Account(account,passwd);
        if(a.first == false)
        {
            
            cout << endl << "验证密码失败，请重新登录..." << endl;
            cout << "还有 " << 2 - count << " 次机会" << endl;
            count ++;
        }
        else 
            break;
        
    }while( count < 3);
    if(count == 3)
    {
        cout << "欢迎下次使用~！" << endl;
        exit(-2);
    }
    cout << endl;
    while(1)
    {
        mg.Menu();
        cout << "请输入您的选择 ： " ;
        cin >> choose;
        switch(choose)
        {
            case 1 : 
                if(a.second.getFlag() == STUDENT)
                {
                    Student *stu = (Student *)& a.second;
                    mg.Stu_Menu(*stu);
                }
                else 
                {
                    cout << "选择身份失败，请重新选择...." << endl;
                    break;
                }                
                break;
            case 2 : 
                if(a.second.getFlag() == TEACHER)
                {
                    Teacher *tea = (Teacher *)& a.second;
                    mg.Tea_Menu(*tea);
                }
                else 
                {
                    cout << "选择身份失败，请重新选择...." << endl;
                    break;
                }
                break;
            case 3 : 
                if(a.second.getFlag() == ADMIN)
                {
                    Admin *ad = (Admin *)& a.second;
                    mg.Adm_Menu(*ad);
                }
                else 
                {
                    cout << "选择身份失败，请重新选择...." << endl;
                    break;
                }
                break;
            case 0 : 
                cout << "欢迎下次使用 ~"  << endl;
                return 1;
                break;
            default : 
                break;
        } 
        
    }
    return 0;
}

char getch()
{
	char c;
	system("stty -echo");  //不回显
	system("stty -icanon");//设置一次性读完操作，如使用getchar()读操作，不需要按enter
	c=getchar();
	system("stty icanon");//取消上面的设置
	system("stty echo");//回显
	return c;
}
void Getpasswd(char * passwd)
{
    int c;
    int n = 0;
    do
    {
        
        c = getch();
        
        if(c != '\n' && c != '\r' && c != 127)
        {
            passwd[n] = c;
            cerr <<'*';
            n++;
        }
        else if((c != '\n' | c != '\r' ) && c == 127)
        {
            if(n > 0)
            {
                n --;
                cerr << '\b';
                cerr << ' ';
                cerr << '\b';
            }
        }
    } while (c != '\n' && c != '\r');
    passwd[n] = '\0';    
}