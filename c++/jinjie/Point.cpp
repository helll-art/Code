#include<iostream>
using namespace std;
class Point
{
        int x;
        int y;
    public :
        friend ostream & operator << (ostream & out, const Point p);
        Point(int X, int Y) : x(X) , y(Y) {}
        Point & operator ++ () ;  // 前置
        Point operator ++ (int) ; //后置
};
Point & Point ::   operator ++ ()
{
    x ++;
    y ++;
    return *this;
}
Point Point :: operator ++(int)
{
    Point temp = * this;
    ++(*this);
    return temp;
}
ostream & operator << (ostream & out, const Point p)
{
    cout << p.x << "  " << p.y;
    return out;
}
int main(int argc, char const *argv[])
{
    Point p (5,10);
    
    
    cout << p++ << endl;
    cout << ++p << endl;
    
    return 0;
}
