#include<iostream>
using namespace std;
/* 
    虚基类用于解决菱形类问题
    Base1,Base2 在继承Base0是创建出两份var
    虚基类让Base1，Base2的父类成员变为指针，只创建出一个父类成员，并且让这两个指针同时指向父类的成员
 */
/* class Base0
{
    public : 
        int var;
        Base0(){cout <<"Base0 " << endl;}
};
class Base1 : virtual public Base0
{
    public : 
        string name;
    Base1(){cout <<"Base1 " << endl;}
};
class Base2 : virtual public Base0
{
    public : 
        string sex;
        Base2(){cout <<"Base2 " << endl;}
};
class Base3 : public Base1 ,public Base2
{
    public : 
        string aaa;
        Base3(){cout <<"Base3 " << endl;}
};
int main(int argc, char const *argv[])
{
    Base3 hl;
    hl.Base1 :: var = 5;
    cout << hl.Base2 :: var << endl;
    cout << hl.Base1 :: var << endl;


    return 0;
} */
/* 

公有派生类对象可以被当做基类的对象使用，反之则不可。
    派生类的对象可以隐含转换为基类对象
    派生类的对象可以初始化基类的引用
    派生类的指针可以隐含转化为基类的指针
通过基类对象名，指针只能使用从基类继承的成员

class Base
{
    public : 
        int age;
        string name;
};
class Base1 : public Base
{
    public : 
        string sex;
};
int main(int argc, char const *argv[])
{
    Base * b = new Base1();
    //b -> sex = "asd";
    Base1 * a = new Base();
    a  -> age = 54;    
    return 0;
}
 */

class Base 
{
    public : 
        int age;
        Base(int Age) : age(Age) { cout << "Base" << endl;}
         ~Base(){cout << "destructing Base" << endl;}

};
class Base1 : public Base
{
    public : 
        int * p;
        Base1(int age,int Age) : Base(Age)
        {
            cout << "Base1" << endl;
            p = new int(age);
        }
         ~Base1()
        {
            cout << "destructing Base1" << endl;
            delete p;
        }
};
int main(int argc, char const *argv[])
{
    Base * p;
    p = new Base1(25,20);
    delete p;
    Base1 * ptr;
    override
    ptr = new Base1(25,20);
    delete ptr;

    
    return 0;
}
