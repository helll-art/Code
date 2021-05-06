#include<iostream>
#include<map>
using namespace std;
/* 
    map中所有的元素都是pair
    pair中的第一个元素为key 键值，起到索引作用，第二个元素为value 实值
    所有元素都会根据元素的键值进行自动排序

    map multimap  属于关联式容器，底层结构是二叉树
    可以根据key 快速找到value
    map 和multimap 的区别
        map 不允许容器中有重复的key值 ，multimap可以


    构造
        map<T1,T2> mp ; 
        map(const map & mp); 
        map & operator = (const map & mp);
    大小和交换
        size(); 返回容器中元素的数目
        empty() ; 是否为空
        swap() ; 交换容器

    插入和删除
        insert(elem) ; 在容器中插入元素
        clear(); 清除所有元素
        erase(pos) ; 删除pos迭代器所指的元素，返回下一个元素的迭代器
        erase(beg,end);
        erase(key);

    查找和统计
        find(key); 朝赵key是否存在，返回该键的元素的迭代器，不存在，返回map.end();
        count(key) ;  统计主键为key的个数
 */

void test01()
{
    map<int,int> mp;
    mp.insert( 1,1);
    for(map<int,int> :: iterator it = mp.begin() ; it != mp.end() ; ++ it)
    {
        cout << it -> first << endl;
    }
}
int main(int argc, char const *argv[])
{
    
    
    return 0;
}
