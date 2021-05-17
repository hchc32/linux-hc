## C++STL
#### STL在哪里?

> STL在c++标准库里,STL占标准库约有85%.
>
> STL在std这个命名空间里面.

###  1.STL六大部件

- 容器(Containers)
- 迭代器(泛化指针)(Itherators)
- 算法(algorithms)
- 适配器(Adapters)
- 仿函数(Functors)
- 分配器(allocators)

### 2.STL六大部件关系

如下图所示





### 3.容器(Containers)

#### 	3.1容器的分类:

- 关系容器(Sequence Containers)

  - Array(数组):不能扩充大小

  - Vector(向量):可以扩充大小

    > 扩充大小方式:每次push_back()一个元素之后,vector所占用的空间就会扩大到原来的两倍,由此可见,有点浪费资源.（代码论证如下）

    ```c++
    /*
    	通过输入不同的value值，来看capacity值的变化.
    */
    #include <iostream>
    #include <vector>
    #include <algorithm>
    using namespace std;
    
    namespace vtest
    {
    void test_vector(long& value)
    {
        vector<string> c;
        char buf[10];
        cout << "test_vector start............." << endl;
        cout << "vector.size() = " << c.size() <<endl;
        clock_t timeStart = clock();
        for(long i = 0 ; i < value ; ++i)
        {
            try
            {
                snprintf(buf,10,"%d",rand());
                c.push_back((string)buf);
            }
            catch(exception& p)
            {
                cout << "i=" << i << " " <<p.what() << endl;
                abort();
            }
        }
        cout << "milli-seconds : " << (clock()-timeStart) << endl;
        cout << "vector.max_size() = " << c.max_size() << endl;
        cout << "vector.size() = " << c.size() <<endl;
        cout << "vector.capacity() = " << c.capacity() << endl;
    }
    }
    int main()
    {
        long value;
        cin >> value;
        vtest::test_vector(value);
        return 0;
    }
    ```

  - Deque(双向队列):Deque分配的内存是分段连续.(并不是真正的连续).每次增加一个buffer.(查更详细的资料)

  - List(双向循环链表)

  - Forword_List(单链表)

  - Slist(老版gunc里的单链表)

- 关联式容器(Unordered Containers)

  - multiset(允许存在重复元素)(底层红黑树实现)
  - multimap(允许存在重复元素)(底层红黑树实现)
  - unordered_multiset(允许存在重复元素)(底层HashTable)
  - unordered_multimap(允许存在重复元素)(底层HashTable)6
  - map(不允许存在重复元素)
  - set(不允许存在重复元素)

  > 关联式容器有自己的find(),比::find()的查找速度快的多.(写代码论证).

  ```c++
/*
   * 1000000
   * test_unordered_set start.............
   * unordered_set.size() = 0
   * milli-seconds : 674424
   * unordered_set.max_size() = 384307168202282325
   * unordered.size() = 998385insert 
   * end .........
   * ::find() milli-seconds : 89815
   * c.find() milli-seconds : 1
   */
  #include <iostream>
  #include <unordered_set>
  #include <algorithm>
  using namespace std;
  namespace umstest
  {
  void test_unordered(long& value)
  {
      unordered_set<string> c;
      char buf[10];
      cout << "test_unordered_set start............." << endl;
      cout << "unordered_set.size() = " << c.size() <<endl;
      clock_t timeStart = clock();
      for(long i = 0 ; i < value ; ++i)
      {
          try
          {
              snprintf(buf,10,"%d",rand());
              c.insert((string)buf);
          }
          catch(exception& p)
          {
              cout << "i=" << i << " " <<p.what() << endl;
              abort();
          }
      }
      cout << "milli-seconds : " << (clock()-timeStart) << endl;
      cout << "unordered_set.max_size() = " << c.max_size() << endl;
      cout << "unordered.size() = " << c.size() <<endl;
      cout << "insert end ........." << endl;
  
      timeStart = clock();
      snprintf(buf,10,"%d",rand());
      string find_value = (string)buf;
      ::find(c.cbegin(),c.cend(),find_value);
      cout << "::find() milli-seconds : " << (clock()-timeStart) << endl;
      timeStart = clock();
      c.find(find_value);
      cout << "c.find() milli-seconds : " << (clock()-timeStart) << endl;
  }
  }
  int main()
  {
      long value;
      cin >> value;
      umstest::test_unordered(value);
      return 0;
  }
  ```
  
  > 对于上面的HashTable来说,它采用的是链地址分配法.(Separate Chaining).
  
  > map作为key-value对,可以用[]的方法来放置元素.
  >
  > 例如:map[key] = value;

#### 3.2为什么没有queue和stack?

由于deque的特点,queue和stack都可以以deque为基础来构造出来.

这种构造是由Containers Adapter 来做的.

![](/home/hchc/Desktop/截图录屏_选择区域_20210308203856.png)

#### 3.3GP与OOP

如何来看待GP与OOP呢？　因为Ｃ＋＋本来就是面向对象的语言,也就是将数据与处理数据的方法封装为一个类，然后可以通过继承和多态的方法，对这个类进行延伸．有一种打包的思想．

而作为STL的GP思想来说，存数据的Containers和处理数据的Algorithm是分开的，这样子分开可以想到，两个部件可以独立开发，互不影响，只需要用Itherators将两者联系起来就可以了．

从我的理解来看，泛型编程主要就在模板，你可以创建数据类型不同的容器，那么对于这些容器的操作，是会有相通之处的，他们的底层原理有的是一样的．所以我们没有必要去对于每个容器都写出自己的method,而是写出通用的method,然后通过Itherator将两者串联起来．

老师,c++是一种oop的语言,但是在c++的STL中,他又要将存数据的容器,和处理数据的方法分开,并没有用到OOP的思想,而是用到了泛型编程(GP)的思想.我想知道老师您怎么看？

##### 3.3.1模板(Template)





#### 3.4List源码解析







 