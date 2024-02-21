#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

template<typename T>
class SkipList {
private:
    //add whatever you want here
    template<typename T>
    class SkipListNode
    {
        
    };

public:
    SkipList();

    ~SkipList();

    // Insert a value into the skip list
    void insert(T item);

    // Search for a value in the skip list
    bool search(T item);

    // Delete a value from the skip list
    void deleteItem(T item);
};

#endif