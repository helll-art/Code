#include<iostream>
#include<vector>

using namespace std;
/* double average(const vector<double> & arr)
{
    double sum = 0;
    for(unsigned i = 0 ; i < arr.size() ; ++ i)
    {
        sum += arr[i];
    }
    return sum / arr.size();
}
int main(int argc, char const *argv[])
{
    int n;
    cout << " n = " ;
    cin >> n;
    vector<double> arr(n);
    cout << "Please input " << n << "real numbers : " << endl;
    for(int i = 0 ; i < n ; ++ i)
    {
        cin >> arr[i];
    }    
    cout << "Average = " << average(arr) << endl;
    

    return 0;
} */



//移动构造和赋值构造
/* class IntNum
{
    public : 
        IntNum(int x = 0) : xptr(new int(x))
        {
            cout << "Calling constuctor..." << endl;
        }
        IntNum(const IntNum & n) : xptr(new int(*n.xptr))
        {
            cout << "Calling copy constructor..." << endl;
        }
        IntNum(IntNum && n) : xptr(n.xptr)
        {
            n.xptr = nullptr;
            cout << "Calling move constructor..." << endl;
        }
        ~IntNum()
        {
            delete xptr;
            xptr = nullptr;
            cout << "Destructing..." << endl;
        }
        int GetInt()
        {
            return *xptr;
        }
    private : 
        int * xptr;
};
IntNum getNum(void )
{
    IntNum a;
    return a;
}
int main(int argc, char const *argv[])
{
    //IntNum a(getNum());
    cout << getNum().GetInt() << endl;

    return 0;
}
 */


class Base1
{
    public : 
        void display() const
        {
            cout << "Base1 :: display()" << endl;

        }

};
class Base2 : public Base1
{
    public : 
        void display() const
        {
            cout <<"Base2 :: display()" << endl;
        }
};
class Derived : public Base2
{
    public : 
        void display() const
        {
            cout << "Derived :: display()" << endl;
        }
};

void fun(Base1 * ptr)
{
    ptr -> display();
}
int main()
{
    Base1 base1;
    Base2 base2;
    Derived derived;
    fun(&base1);
    fun(&base2);
    fun(&derived);
    return 0;
}
