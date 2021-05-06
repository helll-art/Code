#include<algorithm>
#include<iostream>
#include<vector>
using namespace std;

/* 
    copy （iterator beg,iterator end,iterator dest）;
        容器内指定范围的元素拷贝到另一容器中
    replace(iterator beg, iterator end, oldvalue,newvalue);
        将容器内指定范围的旧元素修改为新元素 
    replace_if (iterator beg,iterator end,_Pred,newvalue);
        容器内指定范围满足条件的元素替换为新元素
    swap
        互换两个容器的元素 
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
    vector<int> vec;
    for(int i = 0 ; i < 10 ; ++ i)
    {
        vec.push_back(i);
    }
    vec.push_back(20);
    vec.push_back(20);
    vec.push_back(20);
    vector<int> v;
    v.resize(vec.size());
    copy(vec.begin(),vec.end(),v.begin());
    print(v);
    replace(v.begin(),v.end(),20,200);
    print(v);
}

bool GreaterFive(int val)
{
    return val >= 5;
}
void test02()
{
    vector<int> vec;
    for(int i = 0 ; i < 10 ; ++ i)
    {
        vec.push_back(i);
    }
    vec.push_back(20);
    vec.push_back(20);
    vec.push_back(20);
    replace_if(vec.begin(),vec.end(),GreaterFive,300);
    print(vec);
}
int main(int argc, char const *argv[])
{
    
    test02();
    return 0;
}
