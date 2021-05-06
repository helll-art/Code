#include<iostream>
#include<algorithm>
#include<vector>
using namespace std; 




//for_each
void print(int val)
{
    cout << val << "  " ;
}
class Print
{
    public : 
        void operator()(int val)
        {
            cout << val << "  ";
        }
};
void test01()
{
    vector<int> vec;
    for(int i = 1 ; i < 11 ; i++ )
    {
        vec.push_back(i);
    }
    for_each(vec.begin(),vec.end(),print);
    cout << endl;
    for_each(vec.begin(),vec.end(),Print());
    cout << endl;
}




//transform

int AddFive(int val)
{
    return val + 5;
}
void test02()
{
    vector<int> vec;
    for(int i = 1 ; i < 11 ; i++ )
    {
        vec.push_back(i);
    }

    vector<int> v;
    v.resize(vec.size());
    transform(vec.begin(),vec.end(),v.begin(),AddFive);
    for_each(vec.begin(),vec.end(),print);
    cout << endl;
    for_each(v.begin(),v.end(),print);
}
int main(int argc, char const *argv[])
{
    test02();
    
    return 0;
}
