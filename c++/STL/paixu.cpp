#include<iostream>
#include<list>
#include<vector>
using namespace std;


class Person
{
    public : 
        string name;
        int age; 
        int high;
        Person(string a,int b,int c) : name(a) , age(b) , high(c) {}

};
bool myCompare(Person p1,Person p2)
{
    if(p1.age == p2.age)
    {
        return p1.high > p2.high;
    }
    return p1.age > p2.age;
}
void print(list<Person> &lst)
{
    for(list<Person> :: iterator it = lst.begin() ; it != lst.end() ; it ++ )
    {
        cout << "姓名 ： " << it -> name << "  年龄 ： " << it -> age << "  身高 ： " << it -> high << endl;
    }
    cout << endl;
}
int main(int argc, char const *argv[])
{
    string name;
    int age , high;
    int num;
    list<Person> lst;
    cout << "请输入要添加人的个数 :  " ;
    cin >> num;
    for(int i = 0 ; i < num ; i ++ )
    {
        cout << "第" << i + 1 << "个人" << endl;
        cout << "请输入姓名 ： " ;  cin >> name;
        cout << "请输入年龄 ： " ;  cin >> age;
        cout << "请输入身高 ： " ;  cin >> high;
        lst.push_back(Person(name,age,high));
    }
    lst.sort(myCompare);
    print(lst);
    return 0;
}
