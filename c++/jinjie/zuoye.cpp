#include<iostream>
using namespace std;
class Vehicle
{
    int MaxSpeed;
    double Weight;
    
    public :
        Vehicle(int a,double b) : MaxSpeed(a),Weight(b) { }
        virtual void Run()
        {
            cout << "Vehicle Run " << endl;
        }
        virtual void Stop()
        {
            cout << "vehicle Stop" << endl;
        }
};
class Bicycle : public Vehicle
{   
    int Height;
    public : 
        Bicycle(int h,int a,double b) : Height(h) ,Vehicle(a,b) {} 
        virtual void Run()
        {
            cout << "Bicycle Run " << endl;
        }
        virtual void Stop()
        {
            cout << "Bicycle Stop" << endl;
        }

};
class Car : public Vehicle
{
    int SeatNum ; 
    public : 
        Car(int s,int a ,double b)  : SeatNum(s),Vehicle(a,b) { }
         void Run()
        {
            cout << "Car Run " << endl;
        }
         void Stop()
        {
            cout << "Car Stop" << endl;
        }
};
class Motorcycle : public Bicycle , public Car
{
    public : 
        Motorcycle(int s,int h,int a,double b) : Bicycle(h,a,b) , Car(s,a,b) { }
        virtual void Run()
        {
            cout << "Motorcycle Run " << endl;
        }
        virtual void Stop()
        {
            cout << "Motorcycle Stop" << endl;
        }
};
void func(Vehicle * v)
{
    v -> Run();
    v -> Stop();
}
int main(int argc, char const *argv[])
{
    Motorcycle * mtc = new Motorcycle(2,150,100,1.78);
    Bicycle *bc = new Bicycle (2,20,0.56);
    Car *c = new Car(5,200,3.95);
    func(c); 


    
    return 0;
}

