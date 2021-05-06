#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

/* 
    sort(iterator beg , iterator end , _Pred);
        排序算法，  可以使用函数，也可以使用仿函数
    random_shuffle(iterator beg,iterator end);
        指定范围内的元素随机调整次序
    merge(iterator beg1,iterator end1,iterator beg2,iterator end2,iterator dest);
        两个容器元素合并，并存储到另一容器中
            注意 ： 两个容器必须是有序的
    reverse(iterator beg,iterator end);
        翻转指定范围的元素  
        

 */


void print(vector<int> & vec)
{
    for(vector<int> :: iterator it = vec.begin() ; it != vec.end() ; ++ it)
    {
        cout << * it << "   ";
    }
    cout << endl;
}


//sort(iterator beg , iterator end , _Pred);
bool compare(int val1,int val2)
{
    return val1 > val2;
}
void test01()
{
    int key;
    vector<int> v;
    srand(int(time(NULL)));
    for(int i = 1 ; i < 11 ; ++ i)
    {
        key = rand()%40 + 100;
        v.push_back(key);
    }
    print(v);
    sort(v.begin(),v.end());
    print(v);
    sort(v.begin(),v.end(),compare);
    print(v);
    sort(v.begin(),v.end(),less<>());
    print(v);
}




//random_shuffle(iterator beg,iterator end);


void test02()
{
    vector<int> vec;
    for(int i = 0 ; i < 10 ; ++ i)
    {
        vec.push_back(i);
    }
    print(vec);
    random_shuffle(vec.begin(),vec.end());
    print(vec);
    
}



//merge(iterator beg1,iterator end1,iterator beg2,iterator end2,iterator dest);

void test03()
{
    vector<int> vec1;
    vector<int> vec2;
    for(int i = 0 ; i < 10 ; ++ i)
    {
        vec1.push_back(i);
    }
    print(vec1);
    for(int i = 5 ; i<15 ; ++ i)
    {
        vec2.push_back(i);
    }
    print(vec2);
    vector<int> dest;
    
    
    //必须预先指定出目标容器的大小
    dest.resize(vec1.size() + vec2.size());
    
    
    
    merge(vec1.begin(), vec1.end() , vec2.begin(), vec2.end(),dest.begin());
    print(dest);
}
int main(int argc, char const *argv[])
{
    test03();
    
    return 0;
}
