#include<iostream>
using namespace std;
class Base1 
{
    public : 
        int var;
        void func()
        {
            cout << "Base1" << endl;
        }
};
class Base2
{
    public : 
        int var;
        void func()
        {
            cout << "Base2" << endl;
        }
};
class Dervied : public Base1 ,public Base2
{
    public : 
        int var;
        void func()
        {
            cout << "Dervied" << endl;
        }
};
int main(int argc, char const *argv[])
{
    
    Dervied obj ;
    obj.var = 1;
    cout << obj.var << endl;
    cout << obj.Base1 :: var << endl;
    cout << obj.Base2 :: var << endl;
    obj. Base1 ::func();
    obj.func();
    return 0;
}
