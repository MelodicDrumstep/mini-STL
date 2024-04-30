这里实现了一个链式哈希表 (Linked_hash_map)

### 困难

即之前一直想着是hashmap key 指向 value T. 后来为了满足复杂性才想到直接把value 类型设置为 double list 里的迭代器
```cpp
class linked_hashmap : public hashmap<Key,typename double_list<pair<const Key, T>>::Node*,Hash,Equal>
> //直接继承hashmap
//但是注意，这里我为了能够实现O（1）时间
//find 返回对应的迭代器，我一定要让hashmap
//把key映射到双向链表的一个结点上
//这样我才能返回迭代器
//而不是把key只映射到value的值
```
## 主体

在hash map基础上需要支持“按插入顺序访问元素”这个操作。

本质上就是，整合了一个双向链表和一个哈希表。

大致框架是
```cpp
template<

    class Key,

    class T,

    class Hash = std::hash<Key>,
    //哈希函数通过模板的functor给出
    
    class Equal = std::equal_to<Key>
    //等于函数通过模板

> class linked_hashmap : public hashmap<Key,typename double_list<pair<const Key, T>>::Node*,Hash,Equal>
> //直接继承hashmap
//但是注意，这里我为了能够实现O（1）时间
//find 返回对应的迭代器，我一定要让hashmap
//把key映射到双向链表的一个结点上
//这样我才能返回迭代器
//而不是把key只映射到value的值

{

public:

    typedef pair<const Key, T> value_type;
    
    double_list<value_type> list;
//一个双向链表，按插入顺序来存储
  

// --------------------------

    class const_iterator;

    class iterator

    {

    public:

        typename double_list<value_type>::iterator listIterator;
//这里迭代器用双向链表的迭代器
//这样能实现迭代器++ --， 来实现
//按插入次序访问

        linked_hashmap<Key,T,Hash,Equal> * p2linked_hashmap;
    }
```

用linked hash map可以实现LRU算法，每次找到插入次序排最前面的那个结点。