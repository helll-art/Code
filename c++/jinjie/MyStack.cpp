#include<iostream>
using namespace std;

template<class T,int SIZE  = 50>
class Stack
{
    private : 
        T list [SIZE];
        int top;
    public : 
        Stack();
        void push(const T & item);
        T pop();
        void clear();
        const T & peek() const;
        bool isEmpty() const;
        bool isFull() const;
};

template<class T , int SIZE>
Stack<T,SIZE> :: Stack() : top(-1){}

template<class T,int size>
void Stack<T,size> :: push(const T & item)
{
    list[top++] = item;

}

template<class T, int size>
T Stack<T,size> :: pop()
{
    if(top == -1)
    {
        cout << "Stack Empty!" << endl;
        exit(0);
    }
    return list[top--] ;
}
template<class T,int size>
void Stack<T,size> :: clear()
{
    top = -1;
    return ;
}
template<class T,int size>
const T& Stack<T,size> :: peek() const
{
    return list[top];
}
template<class T,int size>
bool Stack<T,size> :: isFull() const 
{   
    return size - 1 == top; 
}
template<class T,int size>
bool Stack<T,size> :: isEmpty() const 
{
    return top == -1;
}