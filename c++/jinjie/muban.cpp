#include<iostream>
using namespace std;

/* template<class T>
T  abs(T b )
{
    return b < 0 ? -b: b ;
}
int main(int argc, char const *argv[])
{
    int a = -5;
    double b = -5.5;
    cout << abs(a) << endl;
    cout << abs(b) << endl;
    
    return 0;
}
 */
struct Student
{
    int id;
    float gpa;

};

template <class T>
class Store
{
    private : 
        T item;
        bool haveValue;
    public : 
        Store();
        T &getElem();
        void putElem(const T &x);

};
template<class T>
Store<T> :: Store() : haveValue(false){}
template<class T>
T & Store<T> :: getElem()
{
    if(!haveValue)
    {
        cout << "No item present !" << endl;
        exit(1);
    }
    return item;
}
template<class T>
void Store<T> :: putElem(const T & x)
{
    haveValue = true;
    item = x;
}
int main(int argc, char const *argv[])
{
    Store<int> s1,s2;
    s1.putElem(3);
    s2.putElem(-7);
    cout << s1.getElem() << "   " << s2.getElem() << endl;
    Student g = {1000,23};
    Store<Student> s3;
    s3.putElem(g);
    cout << "The Student id is " << s3.getElem().id << endl;
    Store<double> d;
    cout << "retrieving object D...";
    cout << d.getElem() << endl;
    
    return 0;
}
