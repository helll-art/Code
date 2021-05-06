#include<iostream>
using namespace std;
template<class T>
class Node
{
    private : 
        Node<T> * next;
    public : 
        T data;
        Node(const T & item,Node<T> * next = NULL );

        //bool insertAfter(T Data);
        void insertAfter(Node<T> * p);
        Node<T> * deleteAfter();
        Node<T> * nextNode() const;
};

template <class T>
void Node<T> :: insertAfter(Node<T> * p)
{
    p -> next = next;
    next = p;
}
template<class T>
Node<T> * Node<T> :: deleteAfter()
{
    Node<T> * tempptr = next;
    if(next == NULL)
        return 0;
    next = tempptr -> next;
    return tempptr;
     
}
template<class T>
Node<T> * Node<T> :: nextNode() const 
{
    return next;
}