#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T>
class UniquePtr
{
public:
    UniquePtr() : _p(nullptr){};
    UniquePtr(T * pointer) : _p(pointer) {};
    UniquePtr(const UniquePtr & other) = delete;
    UniquePtr& operator= (const UniquePtr & other) = delete;
    operator bool() const
    {
        return _p != nullptr;
    }
    T* get() const
    {
        return _p;
    }
    T operator*() const
    {
        return *_p;
    }
    T* operator->() const
    {
        return _p;
    }
    void reset()
    {
        clear();
    }
    void reset(T * new_pointer)
    {
        clear();
        _p = new_pointer;
    }
    T* release()
    {
        T* temp = _p;
        _p = nullptr;
        return temp;
    }
    ~UniquePtr()
    {
        clear();
    }
    
private:
    void clear()
    {
        delete _p;
        _p = nullptr;
    }
    void copyother(const UniquePtr<T> & other)
    {
        _p = new T(*other);
    }
    T* _p;
};

template <typename T>
UniquePtr<T> make_unique(const T & value)
{
    return UniquePtr<T>(new T(value));
}

#endif //UNIQUE_PTR