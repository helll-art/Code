#include<iostream>
#include<string>
using namespace std;


//赋值，查找，插入，删除，擦去，获取
void test01()
{

    /* 
        string & assign(const char * s);   把字符串s赋值给当前字符串
        string & assign(const char * s,int n);  把字符串s的前n个字符赋值给当前字符串
        string & assign(int n,char c) ; 用n个字符c赋给当前字符串
        string & operator += (const string & str);   
        string & append(const char * s,int n); 把字符串s的前n个字符连接到当前字符串结尾
        string & append(const string & s);
        string & append(const string & s, int pos,int n);   字符串s中从pos开始的n个字符连接到字符串结尾
        int find(const string & str,int pos = 0) const ; 查找str第一次出现的位置，从pos开始查找
        int find(const char * s,int pos ,int n) const ; 从pos位置查找s的前n个字符第一个位置
        int rfind(const string & str,int pos = npos) const ; 查找str最后一次位置，从pos开始查找
        int rfind(const char pos  = npos) const ; 查找str最后一次出现位置，从pos开始查找
        string & replace(int pos,int n,const string & str); 替换从pos开始n个字符为字符串str
        string & replace(int pos,int n,const char * s); 替换从pos开始的n个字符为字符串s
        int compare(const string & s) const ; 与字符串s比较   > 返回 1   < 返回 -1 
        char & at(int n);  通过at方法获取字符
        char & operator[] (int n);  下标方式获取字符
        string & insert(int pos,const char * s) ;  插入字符串
        string & insert(int pos,const string & str) ; 插入字符串
        string & insert(int pos,int n,char c); 在指定位置插入n个字符c
        string & erase(int pos,int n = npos); 删除从pos开始的n个字符
        string substr(int pos = 0, int n = npos) const ; 返回由pos开始的n个字符组成的字符串
     */
    string a = "123123123123123";
    a.assign(5,'1');
    cout << a << endl;
}
int main(int argc, char const *argv[])
{
    
    test01();
    return 0;
}
