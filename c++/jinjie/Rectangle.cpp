#include<iostream>
using namespace std;

class Point
{
    public : 
        void initPoint(float x = 0 , float y = 0) { this -> x = x; this -> y = y;}
        void move(float offX,float offY) { x += offX ; y += offY ;}
        float getX() const {return x ;}
        float getY() const { return y;}
    private : 
        float x,y;
    protected : 
        float z;

};

class Rectangle : private Point
{
    public : 
        void initRectangle(float x,float y, float w,float h)
        {
            initPoint(x,y);
            this -> w = w;
            this -> h = h;
        }
        float getH()  const{return h;}
        float getW() const{return w;}

        //私有继承虽然不能访问基类的成员，但是可以通过公有接口，去得到要的成员值，
        //float getx() const { return getX();}
        //float gety() const { return getY();}
    private : 
        float w,h;
};
int main(int argc, char const *argv[])
{
    Rectangle re;
    re.initRectangle(2,3,4,5);
    cout << re.getX() << endl;
    cout << re.getY() << endl;
    //cout << re.getx() << endl;
    //cout << re.gety() << endl;
    cout << re.getH() << endl;
    cout << re.getW() << endl;
    

    return 0;
}
