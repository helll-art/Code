#include<iostream>
using namespace std;

/* class Complex
{
    public : 
        Complex(double r = 0.0,double i = 0.0) : real(r),imag(i) {}
        Complex operator + ( const Complex & c2) const;
        Complex operator - ( const Complex & c2) const;
        void display() const;
    private : 
        double real;   // 复数实部
        double imag;   // 复数虚部
};
Complex Complex :: operator+(const Complex & c2) const
{
    return Complex(real + c2.real,imag + c2.imag);
}
Complex Complex :: operator-(const Complex & c2) const 
{
    return Complex(real - c2.real,imag - c2.imag);
}
void Complex :: display() const 
{
    cout << "("<< real << "," << imag << ")" << endl;
}
int main(int argc, char const *argv[])
{
    
    return 0;
}
 */
class Complex
{
    public :
        friend Complex operator +(const Complex &A,const Complex &B) ;
        friend void print(const Complex &C);
        friend ostream & operator << (ostream & out,const Complex & c);
        Complex (double Real,double Imag) : real (Real), imag(Imag) {cout <<"..." << endl;}
        Complex() : real(0),imag(0) {cout << "???" << endl;}
        void operator = (const Complex &C)
        {
            this->real = C.real;
            this->imag = C.imag;
        }
    private : 
        double real;
        double imag;
};
Complex operator +(const Complex &A,const Complex &B) 
{
    return Complex(A.real + B.real,A.imag + B.imag);
}
void print(const Complex &C)
{
    cout << C.real << "." << C.imag << endl;
}
ostream & operator << (ostream & out,const Complex & c)
{
    out << "(" << c.real << "," << c.imag << ")" << endl;
}
int main(int argc, char const *argv[])
{
    
    Complex a(1.0,2.0),b(3.0,4.0),c;
    cout << a << endl;
    c = a + b;
    cout << c << endl;
    return 0;
}

 