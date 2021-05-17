## 1.Copy

#### 1.1什么是拷贝呢？

+ 由于数据在计算机中都是二进制数的方式存储的，所以要想把一个数据ｂ拷贝给ａ，就是把ｂ按照一个bit的方式，进行逐个copy.类似于memcpy()函数.

#### 1.2copy ctor

copy　ctor只能有一个,ctor可以有很多重载版本.

+ 我们要知道，在c++中，如果你定义一个类，但是没有写他的copy ctor或者是ctor的时候，编译器会默认给你一个默认的copy ctor或者ctor．
+ 对于默认的ctor来说，他只是把变量的值copy过去，这种叫做浅拷贝．但是对于一些指针，string等，我们不希望这样子，因为浅拷贝会使得两个类中的变量在同一个地址，不安全．所以我们需要深拷贝，就是将原来的指针变量重新复制一份出来，让原有对象和新生对象相互独立，互不影响.
+ 所以有时候我们需要自己去写copy ctor.
+ 说了这么多,看一看copy ctor的定义

> 如果一个构造函数的第一个参数是一个自身类类型的引用，且任何额外参数都有默认值，则这种构造函数是拷贝构造函数.

```c++
class Foo
{
public:
    Foo();          //默认构造函数
    Foo(const Foo&) //拷贝构造函数
    
}
```

##### 1.2.1为什么copy ctor的参数必须是&呢？

> 假如copy ctor的参数不是引用类型，那么实参就要copy一份传给形参，就会调用copy ctor，然后为了拷贝实参，又要调用copy ctor..............................

##### 1.2.2为什么要有移动构造函数呢？

move ctor　不抛出异常．尤其是对vector.

> 从大的方面来讲，c++本质追求的效率．所以每一次的优化，大部分都是为了效率变得更高.

> 由于拷贝构造函数设计到内存拷贝，举一个例子来说，对于STL中的vector来说，如果当前vector的大小为10 ,那么你要push_back第11个元素的时候，他会重新寻找一个大小为20的空间，然后将前10个元素一起拷贝过去，然后再添加第11个元素.
>
> 仔细想一想，如果数据量过大的话，这种拷贝会非常的浪费时间，所以c++11引入了move ctor.

#### 1.3move ctor(c++ 11)

> move ctor不会将原来的内存空间重新拷贝一份，而是将原来类中所有成员的管理权交给了复制的对象．省去了拷贝的时间与资源．从而提升效率．
>
> 从这里可以提及到深浅复制的问题．
>
> 其实不是所有的类都需要写copy ctor，有指针的需要自己写，因为要避免，两个指针指向同一个内存空间的问题，有可能引发delete两次的问题．这就是浅复制，他只是把原成员变量的值赋给新的成员.

```c++
#include <iostream>
#include<vector>
#include<cstring>
using namespace std;
class Str
{
public:
    char* str;
    Str(char value[])
    {
        cout << "普通构造函数..." << endl;
        str = nullptr;
        int len = strlen(value);
        str = new char[len+1];       
        strcpy(str,value);
    }
    Str(const Str& s)
    {
        cout << "拷贝构造函数..." << endl;
        str = nullptr;
        int len = strlen(s.str);
        str = new char[len+1];
        strcpy(str,s.str);
    }
    Str(Str &&s)
    {
        cout << "移动构造函数..." << endl;
        str = nullptr;
        str = s.str;
        s.str = nullptr;
    }
    ~Str()
    {
        cout << "析构函数..." << endl;
        if(str != nullptr)
        {
            delete []str;
            str = nullptr;
        }
    }
};

int main()
{
    char value[] = "l love linux";
    Str s(value);
    vector<Str> vs;
    //ｓ当前为左值
    vs.push_back((s));
    //s变为了亡值(右值)
    vs.push_back(std::move(s));
    cout << vs[0].str << endl;
    if(s.str != nullptr)
    {
        cout << s.str << endl;
    }
    return 0;
}
```

## 2.深入c++流

先用一段代码来引入流的概念:

```c++
//200 10 -50 124x 150
//284
#include <iostream>
using namespace std;
int main()
{
    int input;
    int sum = 0;
    while(cin >> input)
    {
        sum+=input;
    }
    cout << sum << endl;
    return 0;
}
```

> 由于流的概念，200 10 -50 124x 150,这是一串数据流，cin在遇到空格，换行符号，tab,space时会跳过，然后把正确的数据放入变量中，遇到不合法数据就停止.

#### 2.1cin是如何进行数据检测的呢?

什么是流？

> 流是一种抽象，它表示在其上执行输入和输出操作的设备。流基本上可以表示为长度不确定的字符的源或目标。（cplusplus）

> 由于流他有四个状态位

1.eofbit: eof一看就是文件末尾标志,该位通常被设置于cin到达文件末尾时发生的错误.

2.failbit: cin未能操作到预期字符,或者访问没有权限的文件.如同上面的123x一样，x就是未能操作到预期字符.

3.badbit:无法诊断的失败破坏流.

4.goodbit:以上三种情况都没有发生．

goodbit位被置０之后，cin就无法执行输入了．

#### 2.2如何设置状态？

如何强制输入呢？当good位为0时依然可以输入数据？

> 强制修改流的某个位的状态

```c++
//函数原型：
void clear(iostate state = goodbit)

//rdstate()返回流当前的错误位
//将括号内的状态叠加到原始状态上
void ios::setstate (iostate state) 
{
  clear(rdstate()|state);
}
```

```c++
#include <iostream>
using namespace std;
int main()
{
    int input;
    int sum = 0;
    while(cin >> input)
    {
        sum+=input;
    }
    cout << "sum = " << sum << endl;
    //数据没有读完，但是遇到非法字符
    if(cin.fail() && !cin.eof())
    {
        cout << "updateing.." << endl;
        cin.clear();
        while(putchar(cin.get()) != '\n')
        {
            continue;
        }
    }
    else//文件末尾
    {
        cout << "eofbit == 1" << endl;
        exit(1);
    }
    return 0;
}
```





１．由于系统调用是操作系统向用户开放的特殊接口，用户态进程可以根据这组接口来获得内核提供的特殊服务．而操作系统提供的系统调用也管理着底层的硬件设备，所以系统调用是用户态进程与硬件设备打交道的接口．



２.操作系统向下作为一个大管家来管理底层的硬件设备，向上对用户提供图形界面，使得用户可以更加方便的使用计算机．并且也向程序员提供系统调用API,使得程序员可以不用管硬件，就可以对硬件进行相关操作．

３．由于POSIX标准定义了操作系统应该为应用程序提供的接口标准．也就是说它已经定义好了函数的参数，函数的返回值，函数的名称，所以函数的内部实现就可以不用管了．



４．因为无论是图形接口，还是命令行接口，都是在操作系统之上的，都有可能访问磁盘上的数据，而访问磁盘数据，就是要和硬件打交道，而操作系统是专门与硬件打交道的，所以我们只需要使用操作系统提供的接口就可以了，因此我们很有可能进行系统调用．



５．系统调用的本质一种中断．更准确的说，它是一种有意的异常，这种异常被称为陷阱．陷阱可以在用户程序和内核之间提供一个像过程一样的接口，这种接口就叫做系统调用．当进程运行到系统调用处的时候，就会陷入到内核态中，然后由操作系统进行处理，处理完成之后，会重新将控制权交给用户态程序，然后程序继续从上一次执行的地方开始执行．



６．很小的程序，也会经过很多的系统调用．会经过很多的文件读写，和文件的映射．底层调用十分复杂．



## 3.私有构造函数(private ctor)

由class到object一般都要经过构造函数的调用，调用构造函数是从类外到类内的操作，如果ctor不是public的，那么就会构造失败.

如果ctor是private的话,就需要用static 成员函数来构造对象,因为static成员是不需要对象作为载体的，只需要class就可以了,且多个对象共用一个static成员．

调用静态成员来访问private ctor,并返回该对象的指针或者引用,从而构造出新对象.

#### 3.1公有的不好？为什么要有私有的？













