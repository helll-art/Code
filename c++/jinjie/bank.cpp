#include<iostream>
#include<vector>
using namespace std;


/* void zhuanzhi(int (*xptr)[3],int n)
{
    int temp;
    for(int i = 0 ; i < n ; i++ )
    {
        for(int j = 0 ; j <= i / 2  ; ++ j)
        {
            temp = xptr[i][j];
            xptr[i][j] = xptr[j][i];
            xptr[j][i] = temp;
        }
    }

}
int main(int argc, char const *argv[])
{
    int k = 0 ;
    int (*ptr)[3];
    ptr = new int[3][3];
    for(int i = 0 ; i < 3 ; ++ i)
    {
        for(int j = 0 ; j < 3 ; ++ j)
        {

            ptr[i][j] = k++;
        }
    }
    
    zhuanzhi(ptr,3);
    for(int i = 0 ; i < 3 ; ++ i)
    {
        for(int j = 0 ; j < 3 ; ++ j)
        {
            cout << ptr[i][j] << " ";
        }
        cout << endl;
    }
    
    return 0;
}

 */

class Employee
{
    public : 
        Employee() : name(nullptr) , address(nullptr) , city(nullptr) , post(nullptr) {}
        Employee(string m_name,string m_address,string m_city,string m_post) : name(m_name) , address(m_address) , city(m_city) , post(m_post) {}
        void display();
        bool change_name();
        string getName()
        {
            return this->name;
        }
        string addName(string m_name) { name = m_name;}
        string getCity() { return city;}
        string getPost() { return post;}
        string getAddress() { return address;}
    private : 
        string name;
        string address;
        string city;
        string post;

};
void Employee :: display()
{
    cout << "---------Message---------" << endl;
    cout << "name : " << getName() << endl;
    cout << "adress : " << getAddress() << endl;
    cout << "city : " << getCity() << endl;
    cout << "post : " << getPost() << endl;

}
bool Employee :: change_name()
{
    string Name;
    cout << "---------Message---------" << endl;
    cout << "name : " << getName() << endl;
    cout << "您要将姓名改变为 ： ";
    cin >> Name ;
    name = Name;
    cout << "修改完后信息为 ： " << endl;
    display();

}
int main(int argc, char const *argv[])
{
    string name;
    string address;
    string city;
    string post;
    cout << "输入信息 ： " << endl;
    cout << "name : " ;
    cin >> name;
    cout << "address : " ; cin >> address;
    cout << "city : " ; cin >> city;
    cout << "post : " ; cin >> post;
    Employee temp(name,address,city,post);
    Employee a;
    //a = temp;
    a.display();
    a.change_name();
    temp.display();
    return 0;
}


