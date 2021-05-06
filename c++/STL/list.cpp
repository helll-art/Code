#include<iostream>
#include<list>
#include<vector>
using namespace std;

/* 
    list 优点
        采用动态存储分配，不会造成内存浪费和溢出
        连标志性插入删除方便，修改指针即可
    list 缺点
        链表灵活，但是空间和时间额外耗费较大    指针域  ，    遍历
 
 
 
    构造
        list<T> lst; 
        list(bg,end);  左闭右开的数据拷贝给本身
        list(n,elem); n个elem给本身
        list(const list & lst); 拷贝构造
    
    赋值和交换
        assign(beg,end);   左闭右开拷贝
        assign(n,elem); n个elem拷贝
        list & operator = (const list & lst); 
        swap(lst); 将lst与本身元素互换
    大小操作
        size(); 返回容器中元素个数
        empty() ;  判断容器是否为空
        resize(num); 重新指定容器长度num，容器边长，则默认值填充，变短，元素被删除
        resize(num,elem); 

    插入删除 
        push_back(elem) ;  尾部加入元素
        pop_back();   删除容器最后一个元素
        push_front(elem);  头部插入元素
        pop_front(); 从容器开头删除元素
        int insert(pos,elem) ;  在pos位置插入elem元素，返回新数据位置
        void insert(pos,,n,elem) ; 在pos位置插入n个elem ，无返回值
        void insert(pos,beg,end); 在pos位置插入左闭右开的数据，无返回值
        clear();  移除容器所有数据
        erase(beg,end);  删除左闭右开的数据，返回下一个数据的位置
        erase(pos); 删除pos位置的数据，返回下一个数据的位置
    
    
        remove(elem); 删除容器所有与elem匹配的元素
    

    数据存取
        front();  返回第一个元素
        back(); 返回最后一个元素
    
    反转 排序
        reverse(); 反转链表
        sort();  链表排序
 */

int main(int argc, char const *argv[])
{
    vector<int> vec;
    vec.push_back(1);
    vec.push_back(1);
    vec.push_back(1);
    list<double> lst(vec.begin(),vec.end());
    for(list<double> :: iterator it = lst.begin() ; it != lst.end() ; it ++)
    {
        cout << * it << endl;
    }
    

    return 0;
}
