#ifndef SHARED_PTR
#define SHARED_PTR

template <typename T>
class SharedPtr
{
public:
    SharedPtr()
    {
        ptr2block = new InforBlock(nullptr);
    }
    SharedPtr(T* p)
    {
        ptr2block = new InforBlock(p);
    }
    ~SharedPtr()
    {
        if(ptr2block != nullptr)
        clear();    
    }
    SharedPtr(const SharedPtr & other)
    {
        copyother(other);
    }
    SharedPtr& operator= (const SharedPtr & other)
    {
        if(this == &other)
        {
            return *this;
        }
        clear();
        copyother(other);
        return *this;
    }
    operator bool()
    {
        return ptr2block -> _p != nullptr;
    }
    size_t use_count() const
    {
        if(ptr2block == nullptr)
        {
            return 0;
        }
        return ptr2block -> share_count;
    }
    T* get() const
    { 
        if(ptr2block == nullptr)
        {
            return nullptr;
        }
        return ptr2block -> _p;
    }
    T operator* () const
    { 
        return *(ptr2block -> _p);
    }
    T* operator->() const
    {
        return ptr2block -> _p;
    }
    void reset()
    {
        clear();
        ptr2block = new InforBlock(nullptr);
    }
    void reset(T* p)
    {
        clear();
        ptr2block = new InforBlock(p);
    }
private:
    void clear()
    {
        ptr2block -> share_count--;
        if(ptr2block -> share_count == 0)
        {
            delete ptr2block;
        }
        ptr2block = nullptr;
    }
    void copyother(const SharedPtr & other)
    {
        ptr2block = other.ptr2block;
        if(ptr2block == nullptr)
        {
            return;
        }
        ptr2block -> share_count++;
    }

class InforBlock
{
friend class SharedPtr;
public:
    InforBlock(T* p) : _p(p)
    {
        if(p == nullptr)
        {
            share_count = 0;
            return;
        }
        share_count = 1;
    }
    ~InforBlock()
    {
        delete _p;
    }
private:
    size_t share_count;
    T* _p;
};

InforBlock* ptr2block;
};

template <typename T>
SharedPtr<T> make_shared(T elem){
    SharedPtr<T> ret(new T(std::forward<T>(elem)));
    return ret;
}

template <typename T, typename... Ts>
SharedPtr<T> make_shared(Ts&& ... args){
    SharedPtr<T> ret(new T(std::forward<Ts>(args) ...));
    return ret;
}
#endif //SHARED_PTR