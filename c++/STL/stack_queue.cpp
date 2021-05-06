#include<iostream>
#include<stack>
using namespace std;

/*
    栈
        构造函数
            stack<T> stk;  stack采用模板类实现，stack对象的默认构造形式
            stack(const stack & stk);   构造拷贝函数
        赋值
            stack & operator = (const stack & stk); 
        数据存取
            push(elem); 入栈
            pop(); 出站
            top(); 返回栈顶元素
        大小操作
            empty();  判断栈是否为空
            size(); 返回栈的大小
        

    队列
        构造函数
            queue<T> que; 
            queue(const queue & que);
        赋值
            queue & operator = (const queue & que);
        数据存取
            push(elem);
            pop();
            bac();
            front();
        大小操作
            empty(); 
            size();
 */

void test01()
{
    stack<int> stk;
    stk.push(1);
    stk.push(2);
    stk.push(3);
    cout << stk.top() << endl;
    cout << stk.size() << endl;
}
int main(int argc, char const *argv[])
{
    test01();
    
    return 0;
}
