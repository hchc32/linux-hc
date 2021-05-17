## C++STL
#### STL在哪里?

> STL在c++标准库里,STL占标准库约有85%.
>
> STL在std这个命名空间里面.

###  1.STL六大部件:

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

    > 扩充大小方式:每次push_back()一个元素之后,vector所占用的空间就会扩大到原来的两倍,由此可见,有点浪费资源.(写代码论证)

  - Deque(双向队列):Deque分配的内存是分段连续.(并不是真正的连续).每次增加一个buffer.(查更详细的资料)

  - List(双向循环链表)

  - Forword_List(单链表)

  - Slist(老版gunc里的单链表)

- 关联式容器(Unordered Containers)

  - multiset(允许存在重复元素)(底层红黑树实现)
  - multimap(允许存在重复元素)(底层红黑树实现)
  - unordered_multiset(允许存在重复元素)(底层HashTable)
  - unordered_multimap(允许存在重复元素)(底层HashTable)
  - map(不允许存在重复元素)
  - set(不允许存在重复元素)

  > 关联式容器有自己的find(),比::find()的查找速度快的多.(写代码论证).

  > 对于上面的HashTable来说,它采用的是链地址分配法.(Separate Chaining).

  > map作为key-value对,可以用[]的方法来放置元素.
  >
  > 例如:map[key] = value;

#### 3.2为什么没有queue和stack?

由于

 