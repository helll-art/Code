#include<algorithm>
#include<iostream>
#include<vector>
using namespace std;

/* 
    查找算法
        find, count  只能查找相同的元素，
        find_if , count_if  可以查找不符合条件的元素



        
        find(iterator beg,iterator end,value)  查找元素                           
                    查找指定元素，返回指定元素迭代器，找不到返回结束迭代器
        find_if(iterator beg,iterator end,_Pred);  按条件查找元素
                    按条件查找，返回指定元素迭代器，找不到返回结束迭代器
        adjacent_find（iterator beg,iterator end）   查找相邻元素
        bool binary_search(iterator beg,iterator end,value);   二分查找法
            查找指定元素，查到返回true 否则返回false
            注意 ： 在无序序列中不能用
        int count（iterator beg, iterator end,value） 统计元素个数

        int count_if(iterator beg, iterator end, _Pred)   按条件统计元素个数
 */




class Person
{
    public : 
        string name;
        int age;
        Person(string a,int b) : name(a) , age(b) {}
        bool operator == (const Person & p1)
        {
            if(this -> name == p1.name && this -> age == p1.age)
                return true;
            else 
                return false;
        }
};
void test01()
{
    vector<Person> vec;
    vec.push_back(Person("aaa",1));
    vec.push_back(Person("bbb",2));
    vec.push_back(Person("ccc",3));
    vec.push_back(Person("ddd",4));
    vec.push_back(Person("eee",5));
    vec.push_back(Person("ccc",3));
    Person a("ccc",3);
    vector<Person> :: iterator pos = find(vec.begin(),vec.end(),a);
    if(pos != vec.end())
    {
        cout << "查找成功" << endl;
        for( ; pos != vec.end() ; pos ++)
        {
            cout << " name = " << pos -> name << "   age  = " << pos -> age << endl; 
        }
    }
    else 
    {
        cout << "查找失败" << endl;
    }

}
int main(int argc, char const *argv[])
{
    test01();
    
    return 0;
}

