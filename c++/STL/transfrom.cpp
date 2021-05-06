#include<iostream>
#include<vector>
#include<iterator>
#include<algorithm>
#include<functional>
using namespace std;
int main(int argc, char const *argv[])
{
    const int N = 5 ;
    vector<int> s(N);
    for(int i = 0 ; i < N ; i++ )
        cin >> s[i];
    transform(s.begin(),s.end(),ostream_iterator<int>(cout," "),negate<int>());
        //                              迭代器                         函数对象
    cout << endl;
    
    return 0;
}
