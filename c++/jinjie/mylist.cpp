#include<iostream>
using namespace std;

template<class T>
class Node
{
    private : 
        Node<T> * next;
    public :
        T data;
        Node(const T & a,Node<T> * b = 0) : data(a) ,next(b){ }
        Node<T> *deleteNode()
        {
            if(next == nullptr)
            {
                return nullptr;
            }
            Node<T> * tempptr = next;
            next = tempptr -> next;
            return tempptr;
        }
        void insertNode( Node<T> * a)
        {

            a -> next = next;
            next = a;

        }

        const Node<T> * nextNode()
        {
            return next;

        }
};
template<class T>
class LinkedList
{
    private : 
        Node<T> * front,* rear;   //表头表尾指针
        Node<T> * prevPtr, * currPtr; //记录表当前遍历位置的指针，由插入和删除操作更新
        int size;   //表中元素个数
        int position;  //当前元素在表中的位置序号，有reset使用
        //生成 新节点，数据域为item ，指针域为ptrNext
        Node<T> * newNode(const T & item,Node<T> * ptrNext = NULL);
        void freeNode(Node<T> * p);//释放节点
        //将链表L赋值到当前表，被赋值构造函数， operator = 调用
        void copy(const LinkedList<T> & L);

    public : 
        LinkedList();
        LinkedList(const LinkedList<T> & L);
        ~LinkedList();
        LinkedList<T> & operator = (const LinkedList<T> & L);
        int getSize() const; // 返回链表中元素个数
        bool isEmpty() const; // 链表是否为空
        void reset(int pos = 0); //初始化游标的位置
        void next();//使游标移动到下一个节点
        bool endOfList() const;//游标是否到了链表尾
        int currentPosition() const; // 返回游标当前的位置
        void insertFront(const T & item);//在表头插入节点
        void insertRear(const T & item);//在表尾添加节点
        void insertAt(const T & item); // 在当前节点之前插入节点
        void insertAfter(const T & item);  // 在当前节点之后插入节点
        T deleteFront(); //删除头结点
        void deleteCurrent() ; //删除当前节点
        T & data() ; //返回对当前节点成员数据的引用
        const T & data() const; //返回对当前节点成员数据的常引用
        void clear(); //清空链表，释放所有节点的内存空间，被析构函数，operator 

};
template < class T>
Node<T> * LinkedList<T> :: newNode(const T & item,Node<T> * ptrNext = NULL)
{
    Node<T> * p;
    p = new Node <T> (item,ptrNext);
    if(p == nullptr)
    {
        cout << "Memory allocation failure ! "<< endl;
        exit(1);
    }
    return p;
}

template<class T>
void LinkedList<T> :: freeNode(Node<T> * p)
{
    delete p;
}
template<class T>
void LinkedList<T> :: copy(const LinkedList<T> & L)
{
    Node<T> * p = L.front;
    int pos;
    while(p != nullptr)
    {
        insertRear(p -> data);
        p = p -> nextNode();
    }
    if(position == -1)
    {
        return ;
    }
    prevPtr = nullptr;
    currPtr = front;
    for(pos = 0;pos != position ; pos ++ )
    {
        prevPtr = currptr;
        currPtr = currPtr -> nextNode();
    }

}

template<class T>
LinkedList<T> :: LinkedList() : front(nullptr),rear(nullptr),prevPtr(nullptr),currPtr(nullptr),size(0),position(-1) { }
template<class T>
LinkedList<T> :: LinkedList(const LinkedList<T> & L)
{
    front = rear = nullptr;
    prevPtr = currPtr = nullptr;
    size = 0;
    position = -1;
    copy(L);
}
template<class T>
LinkedList<T> :: ~LinkedList()
{
    clear();
}
template<class T>
LinkedList<T> & LinkedList<T> :: operator = (const LinkedList<T> & L)
{
    if(this == & L)
    {
        return * this;
    }
    clear();
    copy(L);
    return * this;
}
 
template<class T>
int LinkedList<T> :: getSize() const 
{   
    return size;
}
template<class T>
bool LinkedList<T> :: isEmpty() const 
{
    return size == 0;
}
template<class T>
void LinkedList<T> :: reset(int pos = 0 )
{
    int startPos;
    if(front == NULL)
        return ;
    if(pos < 0 || pos > size - 1)
    {
        cerr << "Reset : Invalid list position : " << pos << endl;
        return ;
    }
    if(pos == 0)
    {
        prevPtr = nullptr;
        currPtr = front;
        position = 0;
    }
    else 
    {
        currPtr = front -> nextNode();
        prevPtr = front;
        startPos = 1;
        for(position = startPos ; position != pos ; position ++ )
        {
            prevPtr = currPtr;
            currPtr = currPtr -> nextNode();
        }
    }
}

template<class T>
void LinkedList<T> :: next()
{
    if(currPtr != NULL)
    {
        prevPtr = currPtr;
        currPtr = currPtr -> nextNode();
        position ++ ;
    }
}
template<class T>
bool LinkedList<T> :: endOfList() const 
{
    return currPtr == NULL;

}

template<class T>
int LinkedList<T> :: currentPosition() const
{
    return position;
}

template < class T>
void LinkedList<T> :: insertFront(const T & item)
{
    if(front != NULL)
        reset();
    insertAt(item);
}
template<class T>
void LinkedList<T> :: insertRear(const T & item)
{
    Node<T> * nNode;
    prevPtr = rear;
    nNode = newNode(item);
    if(rear == NULL)
        front = rear = nNode;
    else
    {
        rear -> insertAfter(nNode);
        rear = nNode;
    }
    currPtr = rear;
    position = size;
    size ++;
}

template<class T>
void LinkedList<T> :: insertAt(const T & item)
{
    Node<T> * nNode;
    if(prevPtr == NULL)
    {
        nNode = newNode(item,front);
        front = nNode;
    }
    else 
    {
        nNode = newNode(item);
        prevPtr -> insertAfter(nNode);
    }
    if(prevPtr == rear)
    {
        rear = nNode;
        position = size;
    }
    currPtr = nNode;
    size ++ ;
}
template<class T>
void LinkedList<T> :: insertAfter(const T & item)
{
    Node<T> * p;
    p = newNode(item);
    if(front == NULL)
    {
        front = currPtr = rear = p;
        position = 0;
    }
    else
    {
        if(currPtr == NULL)
            currPtr = prevPtr;
        currPtr -> insertAfter(p);
        if(currPtr == rear)
        {
            rear = p;
            position = size;
        }
        else 
            position ++ ;
        prevPtr = currPtr;
        currPtr = p;
    }
    size ++ ;
}

int main(int argc, char const *argv[])
{
   
    
    return 0;
}
