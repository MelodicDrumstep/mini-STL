#ifndef UNIQUE_PTR
#define UNIQUE_PTR

template <typename T>
class UniquePtr
{
public:
    explicit UniquePtr(T * pointer = nullptr) : _p(pointer) {};
    ~UniquePtr()
    {
        delete _p;
    }
    UniquePtr(UniquePtr && other) noexcept : _p(other._p)
    {
        other._p = nullptr;
    }
    UniquePtr(UniquePtr& other) = delete;
    UniquePtr& operator= (UniquePtr & other) = delete;
    UniquePtr& operator= (UniquePtr && other) noexcept
    {
        if(this != &other)
        {
            delete _p;
            _p = other._p;
            other._p = nullptr;
        }
        return *this;
    }
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
    
private:
    void clear()
    {
        delete _p;
        _p = nullptr;
    }
    T* _p;
};

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args)
{
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

#endif //UNIQUE_PTR