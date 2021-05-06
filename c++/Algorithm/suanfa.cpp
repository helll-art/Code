#include<numeric>
#include<algorithm>
#include<iostream>
#include<vector>
using namespace std;



/* 
    accumulate(iterator beg,iterator end,value);
        计算容器元素累计总和
        value 起始值
    fill(iterator beg, iterator end, value);
        向容器中填充元素，value 填充的值
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
        vec.push_back(1);
    }
    int num = accumulate(vec.begin(),vec.end(),0);
    cout << num << endl;
    fill(vec.begin(),vec.end(),4);
    print(vec);
}
int main(int argc, char const *argv[])
{
    test01();
    /* code */
    return 0;
}


