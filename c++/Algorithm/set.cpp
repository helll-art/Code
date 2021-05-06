#include<numeric>
#include<algorithm>
#include<iostream>
#include<vector>
using namespace std;

/* 

    set_intersection(iterator beg1, iterator end1,iterator beg2,iterator end2,iterator dest); 求两个容器的交集
        返回目标容器最后一个元素的迭代器地址
    set_union        求两个容器的并集   两个集合必须有序
        返回目标容器最后一个元素的迭代器地址
    set_difference   求两个容器的差集
        返回目标容器最后一个元素的迭代器地址
 */
void print(vector<int> & vec)
{
    for(vector<int> :: iterator it = vec.begin() ; it != vec.end() ; ++ it)
    {
        cout << * it << "   ";
    }
    cout << endl;
}
void test01()
{
    vector<int> v1;
    vector<int> v2;
    for(int i = 0 ; i < 10 ; ++ i)
    {
        v1.push_back(i);
    }
    for(int i = 5 ; i < 15 ; ++ i)
    {
        v2.push_back(i);
    }
    print(v1);
    print(v2);
    vector<int> dest;
    dest.resize(v1.size() + v2.size());
    cout << "交集" << endl;
    vector<int> :: iterator End = set_intersection(v1.begin(),v1.end(),v2.begin(),v2.end(),dest.begin());
    for(vector<int> :: iterator it = dest.begin() ; it != End ; ++ it)
    {
        cout << * it << "   ";
    }
    vector<int>(dest).swap(dest);
    cout << "????" << dest.size() << endl;
    cout << endl;
    cout << "并集" << endl;
    End = set_union(v1.begin(),v1.end(),v2.begin(),v2.end(),dest.begin());
    for(vector<int> :: iterator it = dest.begin() ; it != End ; ++ it)
    {
        cout << * it << "   ";
    }
    cout << endl;
    cout << "差集" << endl;
    End = set_difference(v1.begin(),v1.end(),v2.begin(),v2.end(),dest.begin());
    for(vector<int> :: iterator it = dest.begin() ; it != End ; ++ it)
    {
        cout << * it << "   ";
    }
    cout << endl;

    
}
int main(int argc, char const *argv[])
{
    test01();
    
    return 0;
}
