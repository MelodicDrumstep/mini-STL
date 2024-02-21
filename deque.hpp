#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <iostream>

namespace sjtu {
const int CAP = 650;
int num_of_cell = 650;

template<class T>
struct block
{
    int size;
    int head, tail;
    T** element;
    block()
    {
    size = 0;
    head = tail = -1;
    element = new T*[num_of_cell];
    for(int i = 0; i < num_of_cell; i++)
    {
      element[i] = NULL;
    }
    }
    block(block & other)
    {
    size = other.size;
    head = other.head;
    tail = other.tail;
    element = new T*[num_of_cell];
    for(int i = 0; i < num_of_cell; i++)
    {
      element[i] = NULL;
    }
    for(int i = 0; i < num_of_cell; i++)
    {
       if(other.element[i] != NULL)
       {
        element[i] = new T(*other.element[i]);
       }
    }
    }
    ~block()
    {
      bdes();
    }
    void bdes()
    {
      if(element != NULL)
      {
        for(int j = 0; j < num_of_cell; j++)
        {
          if(element[j] != NULL)
          {
            delete element[j];
          }
          element[j] = NULL;
        }
        delete element;
        element = NULL;
      }
    }
    void operator=(block & other)
    {
      if(&other == this)
      {
        return;
      }
    size = other.size;
    head = other.head;
    tail = other.tail;
    bdes();
    element = new T*[num_of_cell];
    for(int i = 0; i < num_of_cell; i++)
    {
      element[i] = NULL;
    }
    for(int i = 0; i < num_of_cell; i++)
    {
      if(other.element[i] != NULL)
      {
        element[i] = new T(*other.element[i]);
      }
    }
    }
};
//I want to implement deque as a circular array of pointers to blocks. Each block is a circular array.
//I should remember to adjust every data member to be private finally

template <class T> class deque {
  
friend class iterator;
friend class const_iterator;

public:

public:
  block<T> ** blocks;
  int headblock, tailblock;
  int num_of_block;
  int Size;
  int size_of_block;

  class const_iterator;
  class iterator {
    friend class const_iterator;
    friend deque;
 private:
    int num;
    deque<T>* d;

  public:
    iterator()
    {
      d = NULL;
      num = -1;
    }
    iterator(deque* de, int n)
    {
      num = n;
      d = de;
    }

    void operator=(iterator other)
    {
      num = other.num;
      d = other.d;
    }

    iterator operator+(const int &n) const 
    {
      if(num + n > d -> Size)
      {
        throw invalid_iterator();
      }
      if(n < 0)
      {
        return *this - (-n);
      }
      iterator it(d, num + n);
      return it;
    }


    iterator operator-(const int &n) const 
    {
      if(num - n < 0)
      {
        throw invalid_iterator();
      }
      if(n < 0)
      {
        return *this + (-n);
      }
      iterator it(d, num - n);
      return it;
    }
    int operator-(const iterator &rhs) const 
    {
        if(d != rhs.d)
        {
          throw invalid_iterator();
        }
        return num - rhs.num;
    }

    iterator &operator+=(const int &n) 
    {
      *this = *this + n;
      return *this;
    }
    iterator &operator-=(const int &n)
    {
        *this = *this - n;
        return *this;
    }

    iterator& operator++() 
    {
        return *this += 1;
    }

    iterator operator++(int) 
    {
        iterator it = *this;
        *this += 1;
        return it; 
    }
    iterator operator--(int) 
    {
      
      iterator it = *this;
      *this -= 1;
      return it;
    }
    iterator &operator--() 
    {
      *this -= 1;
      return *this;
    }
    T &operator*() const 
    {
      if(num >= d -> Size || num < 0)
      {
        throw invalid_iterator();
      }
      int ib, ic;
      find(ib, ic);
      return *(d -> blocks[ib] -> element[ic]);
    }
    void find(int& ib, int& ic)const
    {
      int j = d -> headblock;
      int  po = num + 1;
      while(po > d -> blocks[j] -> size)
      {
        po -= d -> blocks[j] -> size;
        j = (j + 1) % d -> num_of_block;
      }
      ib = j;
      ic = (d -> blocks[j] -> head + po - 1) % num_of_cell;
    }

    T *operator->() const noexcept 
    {
      return &(operator*());
    }
    bool operator==(const iterator &rhs) const 
    {
      if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
      if(rhs.num < 0 && rhs.num >= d -> Size)
      {
        throw invalid_iterator();
      }
      if(d != rhs.d)
      {
          return 0;
      }
      return num == rhs.num;
    }
    bool operator==(const const_iterator &rhs) const 
    {
      if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
      if(rhs.num < 0 && rhs.num >= d -> Size)
      {
        throw invalid_iterator();
      }
       if(d != rhs.d)
      {
          return 0;
      }
      return num == rhs.num;
    }
    bool operator!=(const iterator &rhs) const 
    {
      return !operator==(rhs);
    }
    
    bool operator!=(const const_iterator &rhs) const 
    {
      return !operator==(rhs);
    }
  };

  class const_iterator {
    friend class iterator;
    friend deque;
    private:
     const deque<T>* d;
     int num;
  public:
     const_iterator():d(NULL)
    {
      num = -1;
    }
    const_iterator(const deque* de, int n):d(de)
    {
      num = n;
    }
    const_iterator operator+(const int &n) const 
    {
      if(num + n > d -> Size)
      {
        throw invalid_iterator();
      }
      const_iterator it(d, num + n);
      return it;
    }
    const_iterator operator-(const int &n) const 
    {
      if(num - n < 0)
      {
        throw invalid_iterator();
      }
      const_iterator it(d, num - n);
      return it;
    }
    int operator-(const const_iterator &rhs) const 
    {
       if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
       if(rhs.num < 0 && rhs.num >= d -> Size)
      {
        throw invalid_iterator();
      }
        if(d != rhs.d)
        {
          throw invalid_iterator();
        }
        return num - rhs.num;
    }


    const_iterator &operator+=(const int &n) 
    {
      *this = *this + n;
      return *this;
    }
    const_iterator &operator-=(const int &n)
    {
      *this = *this - n;
      return *this;
    }

    const_iterator& operator++() 
    {
        return *this += 1;
    }

    const_iterator operator++(int) 
    {
        const_iterator it = *this;
        *this += 1;
        return it;
    }
    const_iterator operator--(int) 
    {
      
      const_iterator it = *this;
      *this -= 1;
      return it;
    }
    const_iterator &operator--() 
    {
      return *this -= 1;
    }

    const T &operator*() const 
    {
       if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
      int ib, ic;
      find(ib, ic);
      return *(d -> blocks[ib] -> element[ic]);
    }
    void find(int& ib, int& ic)const
    {
      if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
      int n = num;
      int i = 0;
      int j = d -> headblock;
      int po = n + 1;
      while(po > d -> blocks[j] -> size)
      {
        po -= d -> blocks[j] -> size;
        j = (j + 1) % d -> num_of_block;
      }
      ib = j;
      ic = (d -> blocks[j] -> head + po - 1) % num_of_cell;
    }
    const T *operator->() const noexcept 
    {
        return &(operator *());
    }
    bool operator==(const iterator &rhs) const 
    {
      if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
      if(rhs.num < 0 && rhs.num >= d -> Size)
      {
        throw invalid_iterator();
      }
      if(d != rhs.d)
      {
          return 0;
      }
      return num == rhs.num;
    }
    bool operator==(const const_iterator &rhs) const 
    {
      if(num < 0 && num >= d -> Size)
      {
        throw invalid_iterator();
      }
      if(rhs.num < 0 && rhs.num >= d -> Size)
      {
        throw invalid_iterator();
      }
       if(d != rhs.d)
      {
          return 0;
      }
      return num == rhs.num;
    }
    bool operator!=(const iterator &rhs) const 
    {
      return !operator==(rhs);
    }
    
    bool operator!=(const const_iterator &rhs) const 
    {
      return !operator==(rhs);
    }
  };
  deque() 
  {
    headblock = tailblock = -1;
    num_of_block = CAP;
    Size = 0;
    blocks = new block<T>*[num_of_block];
    for(int i = 0; i < num_of_block; i++)
    {
      blocks[i] = new block<T>;
    }
    size_of_block = 0;
  }
  deque(const deque &other) 
  {
    if(other.Size == 0)
    {
    size_of_block = 0;
    headblock = tailblock = -1;
    num_of_block = CAP;
    Size = 0;
    blocks = new block<T>*[num_of_block];
    for(int i = 0; i < num_of_block; i++)
    {
      blocks[i] = new block<T>;
    }
    return;
    }
    size_of_block = other.size_of_block;
    headblock = other.headblock;
    tailblock = other.tailblock;
    num_of_block = other.num_of_block;
    Size = other.Size;
    blocks = new block<T>*[num_of_block];
    for(int i = 0; i < num_of_block; i++)
    {
      blocks[i] = new block<T>;
    }
    if(headblock == tailblock)
    {
      blocks[headblock] = new block<T>(*other.blocks[headblock]);
    }
    else
    {
      int i = headblock;
      while(i != tailblock)
      {
        blocks[i] = new block<T>(*other.blocks[i]);
        i = (i + 1) % num_of_block;
      }
      blocks[i] = new block<T>(*other.blocks[i]);
    }
  }
  void deconstruct()
  {
    headblock = tailblock = -1;
    Size = 0;
    size_of_block = 0;
    if(blocks != NULL)
    {
      for(int i = 0; i < num_of_block; i++)
      {
        if(blocks[i] != NULL)
        {
        delete blocks[i];
        blocks[i] = NULL;
        }
      }
      delete [] blocks;
      blocks = NULL;
    }
  }
  ~deque()
  { 
    deconstruct();
  }
  deque &operator=(const deque &other) 
  {
    if(&other == this)
    {
      return *this;
    }
    deconstruct();
    if(other.Size == 0)
    {
    size_of_block = 0;
    headblock = tailblock = -1;
    num_of_block = CAP;
    num_of_cell = CAP;
    Size = 0;
    blocks = new block<T>*[num_of_block];
    for(int i = 0; i < num_of_block; i++)
    {
      blocks[i] = new block<T>;
    }
    return * this;
    }
    size_of_block = other.size_of_block;
    headblock = other.headblock;
    tailblock = other.tailblock;
    num_of_block = other.num_of_block;
    Size = other.Size;
    blocks = new block<T>*[num_of_block];
    for(int i = 0; i < num_of_block; i++)
    {
      blocks[i] = new block<T>;
    }
    if(headblock == tailblock)
    {
      *blocks[headblock] = *other.blocks[headblock];
    }
    else
    {
      int i = headblock;
      while(i != tailblock)
      {
        *blocks[i] = *other.blocks[i];
        i = (i + 1) % num_of_block;
      }
      *blocks[i] = *other.blocks[i];
    }
    return *this;
  }
  T &at(const size_t &pos) 
  {
      if(pos >= Size || pos < 0)
      {
        throw invalid_iterator();
      }
      int j = headblock;
      int po = pos + 1;
      while(po > blocks[j] -> size)
      {
        po -= blocks[j] -> size;
        j = (j + 1) % num_of_block;
      }
      return *(blocks[j] -> element[(blocks[j] -> head + po - 1) % num_of_cell]);
  }
  const T &at(const size_t &pos) const
  {
    if(Size == 0)
    {
      throw invalid_iterator();
    }
     if(pos >= Size || pos < 0)
      {
        throw invalid_iterator();
      }
      int j = headblock;
      int po = pos + 1;
      while(po > blocks[j] -> size)
      {
        po -= blocks[j] -> size;
        j = (j + 1) % num_of_block;
      }
      return *(blocks[j] -> element[(blocks[j] -> head + po - 1) % num_of_cell]);
  }
  T &operator[](const size_t &pos) 
  {
    return at(pos);
  }
  const T &operator[](const size_t &pos) const 
  {
    return at(pos);
  }
  /**
   * access the first element.
   * throw container_is_empty when the container is empty.
   */
  const T &front() const 
  {
     if(Size == 0)
    {
      throw 1;
    }
      return at(0);
  }
  /**
   * access the last element.
   * throw container_is_empty when the container is empty.
   */
  const T &back() const 
  {
    if(Size == 0)
    {
      throw 1;
    }
    return at(Size - 1);
  }
  /**
   * return an iterator to the beginning.
   */
  iterator begin() 
  {
    iterator it(this, 0);
    return it;
  }
  const_iterator cbegin() const 
  {
    const_iterator it(this, 0);
    return it;
  }

  /**
   * return an iterator to the end.
   */
  iterator end() 
  {
    iterator it(this, Size);
    return it;
  }
  const_iterator cend() const
   {
    const_iterator it(this, Size);
    return it;
   }

  /**
   * check whether the container is empty.
   */
  bool empty() const 
  {
      return Size == 0; 
  }

  /**
   * return the number of elements.
   */
  size_t size() const 
  {
     return Size;
  }


  void clear() 
  {
    deconstruct();
    blocks = new block<T>*[num_of_block];
    for(int i = 0; i < num_of_block; i++)
    {
      blocks[i] = new block<T>;
    }
  }
  /**
   * insert value before pos.
   * return an iterator pointing to the inserted value.
   * throw if the iterator is invalid or it points to a wrong place.
   */
  void double_space()
  {
    num_of_block *= 2;
    block<T> ** b = new block<T>*[num_of_block]; 
    for(int i = 0; i < num_of_block / 2; i++)
    {
      b[i] = blocks[i];
      blocks[i] = NULL;
    }
    for(int i = num_of_block / 2; i < num_of_block; i++)
    {
      b[i] = new block<T>;
    }
    delete[] blocks;
    blocks = b;
  }
  iterator insert(iterator pos, const T &value) 
  {
      int t = pos.num;
      if(pos.d != this || pos.num < 0 || pos.num > Size)
      {
        throw invalid_iterator();
      }
      if(Size == 0)
      {
        push_back(value);
        return begin();
      }
      while(size_of_block >= num_of_block / 4)
      {
        double_space();
      }
      if(pos == end())
      {
        push_back(value);
        return begin() + t;
      }
      if(pos == begin())
      {
        push_front(value);
        return begin();
      }
      int ib, ic;
      pos.find(ib, ic);
      int j = ib;
      Size++;
      if(blocks[j] -> size == num_of_cell)
      {
        size_of_block++;
        int r;
        //puts("!");
        block<T> *tmp = blocks[(tailblock + 1) % num_of_block];
        for(r = (tailblock + 1) % num_of_block; r != (j + 1) % num_of_block;)
        {
          int last = (r - 1 + num_of_block) % num_of_block;
          blocks[r] = blocks[last];
          r = last;
        }
        blocks[r] = tmp;
        blocks[r] -> element[0] = new T(*blocks[j] -> element[ic]);
        blocks[r] -> size++, blocks[r] -> head = blocks[r] -> tail = 0;
        int index = (ic + 1) % num_of_cell;
        while(index != blocks[ib] -> tail)
        {
          blocks[r] -> tail++;
          blocks[r] -> size++;
          blocks[j] -> size--;
          blocks[r] -> element[blocks[r] -> tail] = new T(*blocks[j] -> element[index]);
          index = (index + 1) % num_of_cell;
        }
        blocks[r] -> tail++;
        blocks[r] -> size++;
        blocks[j] -> size--;
        blocks[r] ->element[blocks[r] -> tail] = new T(*blocks[j] -> element[index]);
        while(blocks[j] -> tail != ic)
        {
          delete blocks[j] -> element[blocks[j] -> tail];
          blocks[j] -> element[blocks[j] -> tail] = NULL;
          blocks[j] -> tail = (blocks[j] -> tail - 1 + num_of_cell) % num_of_cell;
        }
        delete blocks[j] -> element[ic];
        blocks[j] -> element[ic] = NULL;
        blocks[ib] -> element[ic] = new T(value);
        blocks[j] -> tail = ic;
        tailblock = (tailblock + 1) % num_of_block;
         return begin() + t;
      }
      //puts("!!!!");
      blocks[ib] -> size++;
      int r = blocks[ib] -> tail;
      blocks[ib] -> tail = (blocks[ib] -> tail + 1) % num_of_cell;
      if(r == ic)
      {
          blocks[ib] -> element[blocks[ib] -> tail] = new T(*blocks[ib] -> element[r]);
          if(blocks[ib] -> element[ic] != NULL)
          {
            delete blocks[ib] -> element[ic];
          }
          blocks[ib] -> element[ic] = new T(value);
      }
      else if(r != ic)
      {
        r = blocks[ib] -> tail;
        while(r != ic)
        {
        int last;
        last = (r - 1 + num_of_cell) % num_of_cell; 
        if(blocks[ib] -> element[r] != NULL)
        {
          delete blocks[ib] -> element[r];
        }
        blocks[ib] -> element[r] = new T(*blocks[ib] -> element[last]);
        r = last;
        }
      if(blocks[ib] -> element[ic] != NULL)
      {
        delete blocks[ib] -> element[ic];
      }
      blocks[ib] -> element[ic] = new T(value);
       return begin() + t;
    }
     return begin() + t;
  }

  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) 
  {
      int t = pos.num;
      if(pos.d != this || pos.num < 0 || pos.num >= Size)
      {
        throw invalid_iterator();
      }
      if(Size == 0)
      {
        throw 1;
      }
      // puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      // printf("block bound: %d %d\n", headblock, tailblock);
      int tmp = headblock, sum = 0;
      while(tmp != (tailblock + 1) % num_of_cell)
      {
      // printf("size index %d\n", tmp);
      // printf("%d %d\n", blocks[tmp] -> head, blocks[tmp] -> tail);
        sum += blocks[tmp] -> size;
        tmp = (tmp + 1) % num_of_cell;
      }
      //printf("deque size: %d %d\n", Size, sum);
      //puts("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
      int ib, ic;
      pos.find(ib, ic);
      if(Size == 0)
      {
        throw invalid_iterator();
      }
      // if(Size == 1)
      // {
      //   printf("%d %d\n", headblock, tailblock);
      //   printf("%d %d\n", blocks[headblock] -> head, blocks[headblock] -> tail);
      // }
      Size--;
      if(Size == 0)
      {
       // puts("!");
        //std::cout << *(blocks[headblock].element[blocks[headblock].head]) << std::endl;
        if(blocks[headblock] -> element[blocks[headblock] -> head] != NULL)
        {
        delete blocks[headblock] -> element[blocks[headblock] -> head];
        }
        //puts("!");
        blocks[headblock] -> element[blocks[headblock] -> head] = NULL;
        blocks[headblock] -> size = 0;
        blocks[headblock] -> head = -1;
        blocks[headblock] -> tail = -1;
        headblock = tailblock = -1;
        size_of_block = 0;
        //puts("!");
        return begin();
      }
      if(pos == begin())
      {
        Size++;
        pop_front();
        return begin();
      }
      if(ib == tailblock && ic == blocks[tailblock] -> tail)
      {
        Size++;
        pop_back();
        return end();
      }
      if(blocks[ib] -> size == 1)
      {
        delete blocks[ib] -> element[ic];
        blocks[ib] -> element[ic] = NULL;
        blocks[ib] -> size = 0;
        blocks[ib] -> head = -1;
        blocks[ib] -> tail = -1;
        size_of_block--;
        if(ib == tailblock)
        {
          tailblock = (tailblock - 1 + num_of_block) % num_of_block;
          return begin() + t;
        }
        delete blocks[ib];
        while(ib != tailblock)
        {
          int next = (ib + 1) % num_of_block;
          blocks[ib] = blocks[next];
          ib = next;
        }
        blocks[tailblock] = new block<T>;
        tailblock = (tailblock - 1 + num_of_cell) % num_of_block;
        return begin() + t;
      }
      blocks[ib] -> size--;
      int k = ic;
      while(k != blocks[ib] -> tail)
      {
        int next = (k + 1) % num_of_cell;
        if(blocks[ib] -> element[k] != NULL)
        {
          delete blocks[ib] -> element[k];
        }
        blocks[ib] -> element[k] = new T(*blocks[ib] -> element[next]);
        k = next;
      }
      if(blocks[ib] -> element[k] != NULL)
      {
      delete blocks[ib] -> element[k];
      }
      blocks[ib] -> element[k] = NULL;
      blocks[ib] -> tail = (blocks[ib] -> tail - 1 + num_of_cell) % num_of_cell; 
      return begin() + t;
  }

  void push_back(const T &value) 
  {
    if(Size == 0)
    {
      size_of_block = 1;
      headblock = tailblock = 0;
      //T a = new T(value);
      blocks[0] -> element[0] = new T(value);
      blocks[0] -> size = 1;
      blocks[0] -> head = blocks[0] -> tail = 0;
      Size++;
      return;
    }
     while(size_of_block >= num_of_block / 4)
      {
        double_space();
      }
    Size++;
    if(blocks[tailblock] -> size == num_of_cell)
    {
      size_of_block++;
      tailblock = (tailblock + 1) % num_of_block;
      //T a = new T(value);
      blocks[tailblock] -> element[0] = new T(value);
      blocks[tailblock] -> tail = blocks[tailblock] -> head = 0;
      blocks[tailblock] -> size = 1;
      return;
    }
    blocks[tailblock] -> size++;
    blocks[tailblock] -> tail = (blocks[tailblock] -> tail + 1) % num_of_cell;
    //std::cout << blocks[tailblock].tail << std::endl;
    //std::cout << &a << std::endl;
    blocks[tailblock] -> element[blocks[tailblock] -> tail] = new T(value);
  }
  void pop_back() 
  {
      if(Size == 0)
      {
        throw index_out_of_bound();
      }
      delete blocks[tailblock] -> element[blocks[tailblock] -> tail];
      blocks[tailblock] -> element[blocks[tailblock] -> tail] = NULL;
      if(Size == 1)
      {
        size_of_block = 0;
        Size = 0;
        blocks[headblock] -> size = 0;
        blocks[headblock] -> head = -1;
        blocks[headblock] -> tail = -1;
        headblock = tailblock = -1;
        return;
      }
      if(blocks[tailblock] -> size == 1)
      {
        blocks[tailblock] -> size = 0;
        blocks[tailblock] -> head = -1;
        blocks[tailblock] -> tail = -1;
        while(blocks[tailblock] -> size == 0)
        {
          size_of_block--;
          tailblock = (tailblock - 1 + num_of_block) % num_of_block;
        }
      }
      else
      {
        blocks[tailblock] -> size--;
        if(blocks[tailblock] -> tail == 0)
        {
          blocks[tailblock] -> tail = num_of_cell - 1;
        }
        else
        {
          blocks[tailblock] -> tail--;
        }
      }
      Size--;
  }

  void push_front(const T &value) {
    if(Size == 0)
    {
      size_of_block = 1;
      headblock = tailblock = 0;
      blocks[0] -> element[0] = new T(value);
      blocks[0] -> size = 1;
      blocks[0] -> head = blocks[0] -> tail = 0;
      Size++;
      return;
    }
     while(size_of_block >= num_of_block / 4)
      {
        double_space();
      }
    Size++;
    if(blocks[headblock] -> size == num_of_cell)
    {
      size_of_block++;
      if(headblock == 0)
      {
        headblock = num_of_block - 1;
      }
      else
      {
        headblock--;
      }
      blocks[headblock] -> element[0] = new T(value);
      blocks[headblock] -> tail = blocks[headblock] -> head = 0;
      blocks[headblock] -> size = 1;
      return;
    }
    if(blocks[headblock] -> head == 0)
    {
      blocks[headblock] -> head = num_of_cell - 1;
    }
    else
    {
      blocks[headblock] -> head--;
    }
    blocks[headblock] -> size++;
    blocks[headblock] -> element[blocks[headblock] -> head] = new T(value);
  }
 /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() 
  {
      if(Size == 0)
      {
        throw invalid_iterator();
      }
      delete blocks[headblock] -> element[blocks[headblock] -> head];
      blocks[headblock] -> element[blocks[headblock] -> head] = NULL;
      if(Size == 1)
      {
        size_of_block = 0;
        Size = 0;
        blocks[headblock] -> size = 0;
        blocks[headblock] -> head = -1;
        blocks[headblock] -> tail = -1;
        headblock = tailblock = -1;
        return;
      }
      if(blocks[headblock] -> size == 1)
      {
        blocks[headblock] -> size = 0;
        blocks[headblock] -> head = -1;
        blocks[headblock] -> tail = -1;
        while(blocks[headblock] -> size == 0)
        {
          size_of_block--;
          headblock = (headblock + 1)  % num_of_block;
        }
      }
      else
      {
        blocks[headblock] -> size--;
        blocks[headblock] -> head = (blocks[headblock] -> head + 1) % num_of_cell;
      }
      Size--; 
  }
};

} 

#endif