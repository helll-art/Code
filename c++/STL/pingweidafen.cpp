#include<iostream>
#include<vector>
#include<algorithm>
#include<ctime>
using namespace std;


/* 
其他的随机数的范围通式

产生一定范围随机数的通用表示公式是：

要取得[0,n)  就是rand（）%n     表示 从0到n-1的数

要取得[a,b)的随机整数，使用(rand() % (b-a))+ a; 
要取得[a,b]的随机整数，使用(rand() % (b-a+1))+ a; 
要取得(a,b]的随机整数，使用(rand() % (b-a))+ a + 1; 
通用公式:a + rand() % n；其中的a是起始值，n是整数的范围。 
要取得a到b之间的随机整数，另一种表示：a + (int)b * rand() / (RAND_MAX + 1)。 
要取得0～1之间的浮点数，可以使用rand() / double(RAND_MAX)。
 */
class Person
{
    public : 
        string name;
        double average = 0;
        vector<double> vec;
    Person(string a) : name(a) {}
    void dafen()
    {
        cout << name << "所得到所有评委成绩为 ： " << endl;
        for(int i = 0 ; i < 10 ;  ++ i)
        {    
            vec.push_back(rand() %20 + 80);
        }
        for(vector<double> :: iterator it =  vec.begin() ; it != vec.end() ; it ++)
        {
            cout << *it << "   ";
            
        }
        cout << endl;
    }
    void  Average()
    {
        double sum  = 0;
        sort(vec.begin(),vec.end());
        cout << name << "所得的评委成绩为 : " << endl; 
        for(vector<double> :: iterator it = ++ vec.begin() ; it != --vec.end() ; it ++)
        {
            cout << *it << "   ";
            sum = sum + * it;
        }
        average =  sum / 8;
        cout << endl;
    }

};

int main(int argc, char const *argv[])
{
    srand( (double) time (NULL) );
    vector<Person> V;
    V.push_back(Person("aaa"));
    V.push_back(Person("bbb"));
    V.push_back(Person("bbb"));
    V.push_back(Person("bbb"));
    V.push_back(Person("bbb"));
    for(vector<Person> :: iterator it = V.begin() ; it != V.end() ; it ++ )
    {
        it -> dafen();
        it -> Average();
        cout << "姓名 ： " << it -> name << "      平均成绩 ： " << it -> average; 
        cout << endl;
    }
    
    return 0;
}
