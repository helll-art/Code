

数组               
指针
动态分配存储
指针和数组
指针和函数
对象的复制和移动
字符串





二维数组和指针数组的区别
/* 
    
        二维数组的内存地址连续 ，  指针数组地址不连续   指针数组里面存放的是地址，
        指针数组的每行的长短可以自定义，二维数组的每行长短必须相同
 */

指针
/* 
不要将 非静态局部地址用做函数的返回值 
    常量指针 ：  const int * p
                指针是常量，指针的指向不能修改，指向的内容可以修改
    指针常量 ：  int * const p
                

    在子函数中通过动态内存分配new操作取得的内存地址返回给主函数是合法有效的，但是内存分配和释放不在同一级别，要注意不能忘记释放，避免内存泄露
     */

函数指针的定义
    
    /* 
    定义形式 ： 存储类型 数据类型( * 函数指针名)();
    含义  : 函数指针指向的是程序代码存储区  
            int compute(int a,int b, int(*func)(int,int))
            {
                return func(a,b);
            }
            int max(int a,int b)
            {
                return (a > b ) ? a : b ;
            }
            int min(int a,int b)
            {
                return (a > b ) ? b : a ;
            }
            int sum(int a,int b)
            {
                return a + b;
            }
     */
c++ 函数前面和后面使用const的作用
    前面使用const表示返回值为const
    后面使用const表示函数不可以修改class的成员

动态创建多维数组
    new 类型名T[第一维长度][第二维长度]....;
    如果内存申请成功，new运算返回一个指向新分配内存首地址的指针
    例如 ： 
        char (*fp)[3];
        fp = new char[2][3];

int *ptr[3]    ptr[3] 是一个数组， int * 是ptr的类型
int (*ptr)[3]   ptr是一个指针  指向一个含有三个元素的数组

当一个对象被用作右值时，用的是对象的值（内容），当对象被用作左值的时候，用的是对象的身份（在内存中的位置）

智能指针 ：
    unique_ptr :    
        不允许多个指针共享资源，可以用标准库中的move函数转移指针
    shared_ptr :
        多个指针共享资源
    weak_ptr : 
        可赋值shared_ptr ,但其构造或者释放对资源不产生影响

指针常量 ：int * const p : 指针是常量，指向不可变
敞亮指针 ： const int * p  指向常量的指针

/* 使用深层复制构造函数
    返回时构造临时对象，动态分配将临时对象返回到主调函数，然后删除临时对象
使用移动构造函数
    将要返回的局部对象转移到主调函数，省去了构造和删除临时对象的过程

 编译器加入参数 -fno-elide-constructors 

&&是右值引用，函数返回的临时变量是右值
移动构造
IntNum(IntNum && n) : xptr(n.xptr)
        {
            n.xptr = nullptr;
            cout << "Calling move constructor..." << endl;
        }  */

        //getline(cin,city,',');
        //getline(cin,state);
















继承和派生概述                                                                            继承的目的
                                                                                              实现设计与代码的重用
继承与派生是同一过程从不同的角度看                                                           派生的目的
    保持已有类的特性而构造新类的过程成为继承                                                         遇到新问题，原有问题无法解决，对源程序进行改造  吸收基类成员，改造积累成员
    在已有类的基础上新增自己的特性而产生新类的过程成为派生
被继承的已有类成为基类（或父类）
派生出的新类成为派生类（或子类）
直接参与派生出某类的基类成为直接基类
基类的基类甚至更高层的基类成为间接基类                   

默认情况下派生类包括了全部积累中除了构造和析构函数之外的所有成员
如果派生类声明了一个和某基类成员同名的新成员，派生的新成员就隐藏或者覆盖了外层同名成员


公有继承
继承的访问控制
    基类的public和protected成员 ： 访问属性在派生类中保持不变                             
    基类的private成员 ： 不可直接访问                                                                           派生类属性不变，但是访问权限会变，只能访问public成员 
访问权限
    //派生类中的成员函数 ： 可以直接访问基类中的public和protected成员，但是不能直接访问基类的private成员        
    通过派生类的对象 ： 只能访问public成员

私有继承
继承的访问控制
    基类的public和protected成员 : 都以private身份出现在派生类中，
    基类的private成员 ： 不可直接访问
访问权限 
    派生类中的成员函数 ： 可以直接访问基类中的public和protected成员，但不能直接访问基类的private成员
    通过派生类的对象 ： 不能直接访问从基类继承的任何成员


保护继承
继承的访问控制
    基类的public和protected成员，都以protected身份出现在派生类中
    基类的private成员 ： 不可以直接访问
访问权限
    派生类中的成员函数 ： 可以直接访问积累中的public和protected成员，但不能直接访问基类的private成员
    通过派生类的对象 ： 不能直接访问从基类继承的任何成员

protected 成员的特点和作用
    对建立其所在类对象的模块来说，它与private成员的性质相同
    对于其派生类来说，它与public成员的性质相同
    即实现了数据隐藏，又方便继承，实现代码重用

对象不能访问保护类和私有类的任何成员

类型转换
公有派生类对象可以被当做基类的对象使用，反之则不可。
    派生类的对象可以隐含转换为基类对象
    派生类的对象可以初始化基类的引用
    派生类的指针可以隐含转化为基类的指针
通过基类对象名，指针只能使用从基类继承的成员










多态
    双目运算符重载规则
        如果要重载B为类成员函数，使之能够实现表达式oprd1 B oprd2，其中oprd1 为A类对象
        则B应被重载为A类的成员函数，形参类型应该是oprd2 所属的类型
        经过重载后，表达式oprd1 B oprd2 相当于oprd1 . operator B (oprd2)
    前置单目运算符重载规则
        如果要重载U为类成员函数，使之能够实现表达式 U oprd，其中oprd为A类对象，则U应被重载为A类的成员函数，无形参
        经过重载后，表达式 U oprd 相当于 iord.operator U ()
    后置单目运算符重载规则
        如果要重载++ 或-- 为类成员函数，使之能够实现表达式 oprd ++ 或 oprd -- 。其中oprd为A类对象，
        则++或--应被重载为A类的成员函数，且具有一个int类型形参
        经过重载后，表达式oprd++ 相当于oprd.operator ++ (0)
    运算符重载为非成员函数的规则
        双目运算符B重载后
            表达式oprd1 B oprd2
            等同于operator B（oprd1，oprd2）
        前置单目运算符 B 重载后
            表达式 B oprd 
            等同于 operator B（oprd）
        后置单目运算符 ++ -- 重载后
            表达式 oprd b
            等同于 operator B（oprd，0）

虚函数
    虚函数声明只能出现在类定义中的函数原型声明中，而不能在成员函数实现的时候
    在派生类中可以对积累中的成员函数进行覆盖
    虚函数一般不声明为内联函数，因为对虚函数的调用需要动态绑定，而对内联函数的处理是静态的
    虚基类
    当派生类从多个基类派生，而这些积累又有共同基类，则在访问此共同基类的成员时，将产生冗余，并有可能因为冗余带来不一致性
    作用  :  主要用来解决多继承时可能发生的对同一基类继承多次而产生的二义性问题
            为最远的派生类提供唯一的基类成员，而不重复产生多次复制
    注意  ：  在第一级继承时就要将共同基类设计为虚基类

虚表
    每个多态类有一个虚表（virtual table）
    虚表中有当前类的各个虚函数的入口地址
    每个对象有一个指向当前类的虚表的指针（虚指针vptr）
动态绑定的实现
    构造函数中为对象的虚指针赋值
    通过多态类型的指针或引用调用成员函数时，通过虚指针找到虚表，进而找到所调用的虚函数的入口地址
    通过该入口地址调用虚函数


纯虚函数(抽象类，带有纯虚函数叫做抽象类)
    纯虚函数是一个在基类中声明的虚函数，他在该积累中没有定义具体的操作内容，要求各派生类根据实际需要定义自己的版本
    virtual 函数类型 函数名（参数表） = 0;
    例如一个二维图形类，不同的图形有不同的面积求法，所以求面积函数不能写成一样
    纯虚函数和虚函数的区别 ： 
        虚函数可以在基类中进行实现，但是纯虚函数不能在基类中实现
    规范整个类家族的统一对外接口

虚基类解决的是类成员标识二义性和信息冗余问题，而虚函数是实现动态多态性的基础











模板
    函数模板 
    类模板
        如果需要在类模板意外定义其成员函数，则需要采用一下形式 ： 
            template<模板参数表>
            类型名 类名 <模板参数标识符列表> :: 函数名（参数表）
为什么有的函数返回引用
    如果一个函数的返回值是一个对象的值，就是右值，不能成为左值
    如果返回值为引用，由于引用是对象的别名，通过引用可以改变对象的值，因此是左值

链表
    生成链表
    插入节点
    查找节点
    删除节点
    遍历链表
    清空链表

 












STL  

迭代器是算法和容器的桥梁
    迭代器用作访问容器中的元素
    算法不直接操作容器中的数据，而是通过迭代器间接操作
算法和容器独立
    增加新的算法，无需影响容器的实现
    增加新的容器，原油的算法也能适用
迭代器是泛化的指针，提供了类似指针的操作
    输入迭代器
        可以用来从序列中读取数据，如输入流迭代器
    输出迭代器
        允许向序列中写入数据，如输出流迭代器
    前向迭代器
        即是输入迭代器又是输出迭代器，并且可以对序列进行单向的遍历
    双向迭代器
        与前向迭代器相似，但是在两个方向上都可以对数据遍历
    随机访问迭代器
        也是双向迭代器，但能够在序列中的任意两个位置之间进行跳转，
        如指针，使用vector的begin，end函数得到的迭代器
    advance(p,n)
        对p执行n次自增操作
    distance(first,last)
        计算两个迭代器first 和 last的距离，即对first执行多少次 ++ 操作后能够使得 first == last

容器类是容纳，包含一组元素或元素集合的对象
基于容器中元素的组织方式分类 顺序容器，关联容器
按照与容器所关联的迭代器类型分类 ： 可逆容器，随机访问容器

vector
    单端数组
deque容器
    双端数组，可以对头端进行插入删除操作
deque与vector区别
    vector 对于头部的插入删除效率低，数据量越大，效率越低
    deque相对而言，对头部的插入删除速度比vector快
    vector访问元素时的速度会比deque快，

    deque内部有一个中控器，维护每段缓冲区中的内容，缓冲区中存放真实数据
    中控器维护的是每个缓冲区的地址，使得使用deque时像是一片连续的内存空间
    deque容器的迭代器也是支持随机访问的
预先定义的输出流对象

cout 标准输出
cerr 标准错误输出，没有缓冲，发送给他的内容立即被输出
clog 类似于cerr 但是有缓冲，缓冲区满时被输出