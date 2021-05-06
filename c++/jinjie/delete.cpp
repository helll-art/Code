#include<iostream>
using namespace std;
int * newintvar()
{
    int * ptr = new int();
    return ptr;
}
int main(int argc, char const *argv[])
{

    
    int * intptr = newintvar();   
    *intptr =  5;
    delete intptr;
    cout << * intptr << endl;
    return 0;
}