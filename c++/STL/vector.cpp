#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;


/*   
操作
    构造
        vector<T> v; 
        vector(v.begin(),v.end()) ;   左闭右开    将区间内的元素拷贝给本身
        vector(n,elem);   构造函数将n个elem拷贝给本身
        vector(const vector & vec);   拷贝构造函数

    赋值
        vector & operator = (const vector & vec) ; 
        assign(beg,end);  左臂右开
        assign(n,elem) ; 

    容量和大小
        empty() ;  判断容器是否为空
        capacity() ;  容器的容量
        size(); 返回容器中元素的个数
        resize(int num );   重新指定容器的长度为num，若容器边长，则以默认值填充新位置
                            如果容器变短，则末尾超出容器长度的元素被删除
        resize(int num,elem); 容器边长，用elem填充新位置 

    插入和删除
        push_back(ele);  尾部插入元素ele
        pop_back() ;    删除最后一个元素
        insert(const_iterator pos,ele); 迭代器指向位置pos插入元素ele
        insert(const_iterator pos, int count , ele); 迭代器指向位置pos插入count个元素ele
        erase(const_iterator pos); 删除迭代器指向的元素
        erase(const_iterator start,const_iterator end); 删除迭代器从start到end的元素
        clear() ;  删除容器中所有元素
    
    数据存取
        at(int idxx);  返回索引idx所指的数据
        operator[];   返回索引idx所指的元素
        front(); 返回容器中的第一个数据元素
        back();   返回容器中最后一个数据元素

    互换容器
        swap(vec); 将vec与本身的元素互换
        假设v.capacity() = 1000000, v.size() = 3;   现在存在很大的空间被浪费，怎么变小
        vector<int>(v).swap(v); 
            vector<int> (v)  ;  创建匿名对象，并且进行拷贝构造，  拷贝时拷贝的是有效数据，并不会按照v的空间也进行拷贝
                                假设现在的匿名对象名为x ， x.swap(v);  两者所指容器的指针进行互换，所以v得到了x所指向的内存
                                                     x指向v原有的内存，匿名对象的特点，当前行执行完，所占用的内存会进行回收


    预留空间  --------------减少动态扩展容量时的扩展次数
        reserve(int len);   容器预留len个元素长度，预留位置不初始化，元素不可访问

 */
//遍历
void myPrint(int a)
{
    cout << a << endl;
}
void test01()
{
    vector<int> p;
    p.push_back(10);
    p.push_back(20);
    p.push_back(30);
    p.push_back(40);
    vector<int> :: iterator itBegin = p.begin();
    vector<int> :: iterator itEnd = p.end();
    while(itBegin != itEnd)
    {
        cout << *itBegin << " ";
        itBegin ++ ;
    }

    
    for(vector<int> :: iterator it = p.begin() ; it != p.end() ; it ++ )
    {
        cout << *it << " ";
    }
    for(vector<int> :: iterator it = --p.end() ; it != --p.begin() ; it -- )
    {
        cout << * it << " ";
    }



    for_each(p.begin(),p.end(),myPrint);
    //for_each的底层代码中的第三个参数 ，里面写的函数内容就是 利用正常for循环里应有的内容

}







//存放自定义数据类型
class Person
{
    public : 
        string name;
        int age;
        Person(string a,int b) : name(a) , age(b){}
        
};

void myprint(Person & p)
{
    cout << p.name << "   " << p.age << endl;
}
void test02()
{
    vector<Person> v;
    v.push_back(Person("aaa",10));
    v.push_back(Person("aaa",20));
    v.push_back(Person("aaa",30));
    v.push_back(Person("aaa",40));
    for(vector<Person> :: iterator it = v.begin() ; it != v.end() ; it++  )
    {
        cout << it -> age << endl;
    }
    for_each(v.begin(),v.end(),myprint);
}


//容器嵌套    可变二维数组
 
void VectorPrint(vector<int> v)
{
    for(vector<int> :: iterator it = v.begin(); it != v.end() ; it ++ )
    {
        cout << *it << "  ";
    }
    cout << endl;
}
void test03()
{
    vector< vector <int> > V;
    vector<int> v1;
    vector<int> v2;
    vector<int> v3;
    vector<int> v4;
    
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    v1.push_back(4);
    
    v2.push_back(5);
    v2.push_back(6);
    v2.push_back(7);
    
    v3.push_back(8);
    v3.push_back(9);
    
    v4.push_back(10);
    
    V.push_back(v1);
    V.push_back(v2);
    V.push_back(v3);
    V.push_back(v4);

    //*it  是 vector中的内容
    for(vector<vector<int>> :: iterator it = V.begin() ; it != V.end() ; it++)
    {
        for(vector<int> :: iterator its = it -> begin() ; its != it -> end() ; its ++)
        {
            cout << * its << "  ";
        }
        cout << endl;
    }
    //for_each 中自己做了解引用的操作
    for_each(V.begin(),V.end(),VectorPrint);

}
int main(int argc, char const *argv[])
{
    test01();
    
    return 0;
}
