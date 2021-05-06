#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

/* 
    函数对象 仿函数
        函数对象在使用时，可以向普通函数那样调用，可以有参数，可以有返回值
        函数对象超出普通函数的概念，函数对象可以有自己的状态
        函数对象可以作为参数传递

    谓词
        返回bool类型的仿函数成为谓词
        如果operator() 接受一个参数，那么叫做一元谓词
        如果operator() 接受两个参数，那么叫做二元谓词
     */

class MyAdd
{
    public :
        int operator()(int v1,int v2)
        {
            return v1 + v2;
        }
};
//函数对象在使用时，可以向普通函数那样调用，可以有参数，可以有返回值
void test01()
{
    MyAdd myadd;
    cout << myadd(10,10) << endl;
}
//函数对象超出普通函数的概念，函数对象可以有自己的状态
//普通打印函数只能进行打印，但是通过函数对象可以给打印函数外加统计
class Print
{
    public : 
        Print()
        {
            sum = 0;
        }
        void operator()(string text)
        {
            sum ++;
            cout << text << endl;
        }
        int sum;
};
void test02()
{
    Print print;
    print("hello world");
    print("hello world");
    print("hello world");
    print("hello world");
    cout << "cout 共调用 " << print.sum << " 次" << endl;
}
//函数对象可以作为参数传递
void doPrint(Print & p,string text)
{
    p(text);
}
void test03()
{
    Print print;
    doPrint(print,"hello");
}





//一元谓词

class GreatFive
{
    public : 
        bool operator()(int val)
        {
            return val > 5;
        }
};
void test04()
{
    vector<int> vec;
    for(int i = 0 ; i < 10 ;  ++ i)
    {
        vec.push_back(i);
    }
    vector<int> :: iterator pos = find_if(vec.begin(),vec.end(),GreatFive());
    for( ; pos != vec.end() ; ++pos)
    {
        cout << *pos << endl;
    }
}


//二元谓词

int main(int argc, char const *argv[])
{
    //test01();
    //test02();
    //test03();
    /* code */
    test04();
    return 0;
}
