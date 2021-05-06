#include<iostream>
#include<fstream>
using namespace std;

 /*    
    ofstream : 写
    ifstream : 读
    fstream : 读写 
    写文件步骤
        1.包含头文件   #include<fstream>
        2.创建流对象    ofstream ofs;                           ifstream   ifs;            
        3.打开文件     ofs.open("文件路径",打开方式);              ifs.open
        4.写数据       ofs << "写入的数据 " ;                    
        5.关闭文件     ofs.close();

    文件打开方式  
        ios :: in   读文件打开
        ios :: out  写文件打开文件
        ios :: ate  初始位置 ： 文件尾
        ios :: app  追加方式写文件
        ios :: trunc 如果文件存在先删除，再创建
        ios :: binary  二进制方式

        二进制方式写文件  ios :: binary | ios :: out

    文件是否打开   ifs.is_open()

        char buf[1024];
        ifs.getline(buf,sizeof(buf));   按行读取
        
        string buf;
        getline(ifs,buf);

        按字符读取
        char c;
        while((c = ifs.get()) != EOF)


    类进行写入读取
        ofstream ofs;
        ofs . open("Person.txt", ios :: out | ios :: binary);
        Person p("张三", 18);
        ofs.write((const char * ) & p,sizeof(Person));
        ofs.close();

        Person a;
        ofs.read(& a,sizeof(Person));
    */

