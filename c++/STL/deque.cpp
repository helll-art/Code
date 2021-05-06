#include<iostream>
#include<deque>
using namespace std;

/* 
    构造
        deque<T> deqT; 默认构造
        deque(beg,end);  构造函数将左闭右开的元素拷贝给本身
        deque(n,elem); 构造函数将n个elem拷贝给本身
        deque(const deque & deq); 拷贝构造函数

    赋值
        deque & operator = (const deque & deq); 
        assign(beg,end); 将左闭右开区间的数据拷贝赋值给本身
        assign(n,elem); 将n个elem拷贝赋值给本身

    大小操作
        deque.empty();  判断容器是否为空
        deque.size(); 返回容器中元素的个数
        deque.resize(num); 重新指定容器的长度为num，若容器边长，则以默认值填充新位置，
                            如果容器变短，则末尾超出容器长度的元素被删除
        deque.resize(num,elem); 重新指定容器长度num，以elem填充

    插入和删除
        两端插入操作    
            push_back(elem); 在容器尾部添加一个数据
            push_front(elem); 在容器头部插入一个数据
            pop_back();
            pop_front();
        指定位置操作
            insert(pos,elem);  在pos位置插入一个elem元素的拷贝，返回新数据的位置
            insert(pos,n,elem); 在pos位置插入n个elem数据，无返回值
            insert(pos,beg,end); 在pos位置插入左闭右开区间数据，无返回值
            clear(); 清空容器的所有数据
            erase(beg,end); 删除左闭右开的数据，返回下一个数据的位置
            erase(pos); 删除pos位置的数据，返回下一个数据的位置 
    数据存取
        at(int idx); 
        operator[];
        front();   返回第一个元素
        back();    返回最后一个元素
    排序
        sort(iterator beg, iterator end) ;   对beg和end区间内元素进行排序
 */
void test01()
{
    deque<int> deq;
    for(int i = 0 ; i < 10 ; ++ i)
    {
        deq.push_back(i);
    }
    for(deque<int> :: iterator it = deq.begin(); it!= deq.end() ; it ++ )
    {
        cout << *it << "  ";
    }
    cout << endl;
    
}
int main(int argc, char const *argv[])
{
    
    test01();
    return 0;
}
