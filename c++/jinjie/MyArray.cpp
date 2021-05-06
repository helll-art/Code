#include<iostream>
using namespace std;
#include<cassert>

template<class T>
class Array
{
    private : 
        T * list;
        int size; 
    public :
    friend void  test();
        Array(int sz = 60);
        Array(const Array<T> &a);
        ~Array();
        Array<T> & operator =  (const Array<T> &a);
        T& operator [] (int a);
        operator T*();
        int getSize() const;
        void resize(int sz);
};
template<class T>
Array<T> :: operator T *()
{
    return this.list;
}
template<class T>
int Array<T> :: getSize() const
{
    return size;
}
template<class T>
void Array<T> :: resize(int sz)
{
    assert(sz>=0);
    if(size == sz)
    {
        cout <<"修改值与程序当前数组大小相同，无需修改";
        exit(0);
    }
    size = sz;
    delete [] list;
    list = new T[size];
    for(int i = 0 ; i < sz ; ++ i)
    {
        list[i] = 0;
    }
}

template<class T>
T& Array<T> :: operator [ ] (int a)
{
    assert(a >= 0 && a < size);
    return list[a];
}

template<class T>
Array<T> :: Array(int sz)
{
    assert(sz >=0);
    size = sz;
    list = new T [size];
}
template<class T>
Array<T> :: Array(const Array<T> &a)
{
    if(a.size != size)
    {
        size = a.size;
        delete [] list;
        list = new T[size];
        
    }
    for(int i = 0 ; i < a.size ; ++ i)
    {
        list[i] = a.list[i];
    }
}
template<class T>
Array<T> :: ~Array()
{
    if(list != nullptr)
    {
        size = 0;
        delete [] list;
    }
}
template<class T>
Array<T> & Array<T> ::  operator = (const Array<T> & a)
{
    if(size != a.size)
    {
        size  = a.size;
        delete [] list;
        list = new T [size];    
    }
    for(int i = 0 ; i < a.size ; ++ i)
    {
        list[i] = a.list[i];
    }
    return * this;
}

void  test()
{
    Array<int> p(20);
    for(int i = 0 ; i < p.getSize() ; ++ i)
    {
       p.list[i] = i;
       cout << p.list[i] << "  "; 
    }
    cout << endl;
    Array<int> q;
    q = p;
    for(int i = 0 ; i < q.getSize() ; ++ i)
    {
       q.list[i] = i;
       cout << q.list[i] << "  "; 
    }
    cout << q.getSize() << endl;
}
int main(int argc, char const *argv[])
{
    
    test();
    return 0;
}
