#include<iostream>
#include<map>
#include<vector>
#include<list>
#include<time.h>
#include<string>
using namespace std;


class Staff
{
    public : 
        string name = "员工";
        int money;
        Staff(char a,int b) 
        {
            
            name = name + a;
            money = b;
         }

};
void printMessage(multimap<int,Staff> & mp)
{
    /* for(multimap<int,Staff> :: iterator it = mp.begin() ; it != mp.end() ; it ++ )
    {
        cout << "职员部门 ： " ; 
        switch(it -> first)
        {
            case 0 : 
                cout << " 策划";
                break;
            case 1 : 
                cout << " 美术";
                break;
            case 2 : 
                cout << " 研发" ;
                break;
            default : 
                cout << "错误" ;
                break;
        }
        cout << "    姓名 ： " << it -> second.name << "   工资 ： " << it -> second .money  << endl;

    } */

    int index = 0;
    int count = mp.count(0);
    cout <<  "策划部门总人数 ： " << count << endl;
    
    for(multimap<int,Staff> :: iterator pos = mp.find(0); pos != mp.end() /* && index < count */ ; pos ++,index ++ )
    {
        cout << "    姓名 ： " << pos -> second.name << "   工资 ： " << pos -> second .money << endl ;
    }
    index = 0;
    count = mp.count(1);
    cout <<  "美术部门总人数 ： " << mp.count(1) << endl;
    for(multimap<int,Staff> :: iterator pos = mp.find(1); pos != mp.end() && index < count ; pos ++, index ++ )
    {
        cout << "    姓名 ： " << pos -> second.name << "   工资 ： " << pos -> second .money  << endl;
    }
    index = 0;
    count = mp.count(2);
    cout <<  "研发部门总人数 ： " << count << endl;
    for(multimap<int,Staff> :: iterator pos = mp.find(2); pos != mp.end() && index < count; pos ++ , index ++ )
    {
        cout << "    姓名 ： " << pos -> second.name << "   工资 ： " << pos -> second .money  << endl;
    }
} 

int main(int argc, char const *argv[])
{
    int k,money ;
    srand(int (time(NULL)));
    multimap<int,Staff> mp;
    
    for(int i = 0 ; i < 10 ; i++ )
    {
        k = rand() % 3;
        money = rand() % 5000 + 5000;
    
        Staff st(i + 'A',money);
        
        mp.insert(pair<int,Staff>(k,st));
    }

    printMessage(mp);
    return 0;
}
