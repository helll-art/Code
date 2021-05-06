#include<iostream>
using namespace std;
class Base1 
{
    public :
        //virtual void display() const;
        Base1() { cout << "create Base1 " << endl;}
        void display()
        {
            cout << "Base1" << endl;
        }
};
/* 
    虚函数都在类外进行实现
void Base1 :: display() const 
{
    cout << "Base1" <<endl;
} */
class Base2 : public Base1
{
    public :
        Base2() { cout << "create Base2 " << endl;}
        void display()
        {
            cout << "Base2" << endl;
        }
};
class Base3 : public Base2 
{
    public : 
        Base3() { cout << "create Base3" << endl;}
        void display()
        {
            cout << "Base3" << endl;
        } 
};
void fun(Base3 * ptr)
{
    ptr -> display();
}
int main(int argc, char const *argv[])
{
    Base3 b1,b2,b3;
    fun(&b3);
    fun(&b2);
    fun(&b1);
    return 0;
}
