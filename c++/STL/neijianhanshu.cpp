#include<iostream>
#include<vector>
#include<functional>
#include<algorithm>
using namespace std;

/* 

    STL 内建了一些函数对象
        算术仿函数‘’
       关系仿函数
       逻辑仿函数
       这些仿函数产生的对象，用法和一般函数完全相同
       使用内建函数对象，需要引用头文件 #include<functional> 


        算术仿函数
            只有取反为一元仿函数，其余均为二元仿函数
            template<class T> T plus<T>   +
            template<class T> T minus<T>   -
            template<class T> T multiplies<T>  * 
            template<class T> T divides<T>   /
            template<class T> modulus<T>    取模
            template<class T> negate<T>    取反
        关系仿函数
            template<class T> bool equal_to<T>                      ==
            template<class T> bool not_equal_to<T>                  !=
            template<class T> bool greater<T>                       >
            template<class T> bool greater_equal<T>               >=
            template<class T> bool less<T>                        <
            template<class T> bool less_equal<T>                  <=
        逻辑仿函数
        template<class T> bool logical_and<T>  与
        template<class T> bool logical_or<T>   或
        template<class T> bool logical_not<T>  非

 */

//算术仿函数
void test01()
{
    plus<int> n;
    cout << n(20,20) << endl;
    negate<int> m;
    cout << m(50) << endl;
    modulus<int> mo;
    cout << mo(-20,1) << endl;
    
}
//关系仿函数
class MyCompare
{
    public : 
        bool operator()(int val1,int val2)
        {
            return val1 > val2;
        }
};
void tesst02()
{
    vector<int> vec;
    vec.push_back(20);
    vec.push_back(10);
    vec.push_back(30);
    vec.push_back(50);
    vec.push_back(40);
    sort(vec.begin(),vec.end(),greater<int>());
    sort(vec.begin(),vec.end(),MyCompare());
    //利用系统提供的逻辑仿函数，在排序时不需要自己手动进行书写
}


//逻辑仿函数
void print(vector<bool> & vec)
{
    for(vector<bool> :: iterator it = vec.begin() ; it != vec.end() ; it++)
    {
        cout << *it << "   ";
    }
    cout << endl;
}
void test03()
{
    vector<bool> vec;
    vec.push_back(false);
    vec.push_back(true);
    vec.push_back(false);
    vec.push_back(true);
    print(vec);
    vector<bool> v;
    v.resize(vec.size());
    transform(vec.begin(),vec.end(),v.begin(),logical_not<bool>());
    print(v);
}
int main(int argc, char const *argv[])
{
    test03();
    
    return 0;
}
