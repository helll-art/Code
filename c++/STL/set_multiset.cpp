#include<iostream>
#include<set>
using namespace std;


/* 

    set 和 multiset 的区别： 
        set 不允许容器中有重复的元素
        multiset允许容器中有重复的元素
    
    构造
        set<T> st; 
        set(const set & st); 
        set & operator = (const set & st);
    大小和交换
        size();
        empty();
        sqap(st);     
    插入和删除
        insert(elem); 在容器中插入元素
                    pair<set<int> :: iterator, bool> ret = st.insert(10);
                    multiset insert返回的是自身的迭代器
        clear(); 清楚所有元素
        erase(pos); 删除pos迭代器所指的元素，返回下一个元素的迭代器
        erase(beg,end); 删除左闭右开区间的元素,返回下一个元素的迭代器
        erase(elem); 删除容器中值为elem的元素

    查找和统计
        find(key); 查找key是否存在，若存在，返回该键元素的迭代器，若不存在，返回set.end()；
        count(key); 统计key元素个数


    对组
        pair<type , type> p (value1,value2);
        pair<type , type> p = make_pair(value1,value2);

 */
/* 
insert(elem); 在容器中插入元素
                    pair<set<int> :: iterator, bool> ret = st.insert(10);
                    multiset insert返回的是自身的迭代器
 */
void test01()
{
    set<int> st;
    pair<set<int> :: iterator, bool> ret = st.insert(10);
    if(ret.second)
    {
        cout << "第一次插入成功" << endl;
    }
    else
    {
        cout << "第一次插入失败" << endl;
    }
    st.insert(30);
    st.insert(20);
    st.insert(40);
    ret = st.insert(10);
    if(ret.second)
    {
        cout << "第二次插入成功" << endl;
    }
    else
    {
        cout << "第二次插入失败" << endl;
    }
}
class Person
{
    public : 
        string name;
        int age;
        int high;
        Person(string a,int b,int c) : name(a) , age(b) , high(c) {}

};
class MyCompare
{
    public : 
        bool operator()(const Person & p1, const Person & p2)
        {
            return p1.age > p2.age;
        }
};
void test02()
{
    set<Person,MyCompare> st;
    st.insert(Person("aaa",12,12));
    st.insert(Person("bbb",13,13));
    st.insert(Person("ccc",14,14));
    st.insert(Person("ddd",15,15));

    for(set<Person,MyCompare> :: iterator it = st.begin() ; it != st.end() ; it ++)
    {
        cout << "姓名 ： " << it -> name << "   年龄 ： " << it -> age << "   身高 ： " << it -> high << endl;
     } 
}

int main(int argc, char const *argv[])
{
    
    test02();
    return 0;
}
