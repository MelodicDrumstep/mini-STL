#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <functional>

template<typename T, typename Compare = std::less<T>>
class SkipList {
private:
    //add whatever you want here
    class SkipListNode
    {
    public:
        T * key;
        std::vector<SkipListNode *> forward;
        SkipListNode(T * key, int level)
        {
            this -> key = key;
            forward = std::vector<SkipListNode *>(level + 1);
            for(int i = 0; i < level + 1; i++)
            {
                forward[i] = nullptr;
            }
        }
        ~SkipListNode()
        {
            delete key;
        }
    };

    Compare comparator;

    int MAXLVL;
    float P;
    int level;
    SkipListNode * header;

public:
    SkipList()
    {
        this -> MAXLVL = 16;
        this -> P = 0.5;
        level = 0;
        header = new SkipListNode(nullptr, MAXLVL);
    }

    ~SkipList()
    {
        SkipListNode * current = header -> forward[0];
        while(current != nullptr)
        {
            SkipListNode * next = current -> forward[0];
            delete current;
            current = next;
        }
        delete header;
    }

    int randomLevel()
    {
        int lvl = 0;
        while((rand() / (float)RAND_MAX) < P && lvl < MAXLVL)
        {
            lvl++;
        }
        return lvl;
    }
    
    SkipListNode * createNode(const T & item, int level)
    {
        T * key = new T(item);
        SkipListNode * newNode = new SkipListNode(key, level);
        return newNode;
    }

    // Insert a value into the skip list
    void insert(const T & item)
    {
        std::vector<SkipListNode *> update(MAXLVL + 1, nullptr);
        SkipListNode * current = header;
        for(int i = level; i >= 0; i--)
        {
            while(current -> forward[i] != nullptr && comparator(*(current -> forward[i] -> key), item))
            {
                current = current -> forward[i];
            }
            update[i] = current;
        }
        current = current -> forward[0];
        if(current != nullptr && !comparator(*(current -> key), item) && !comparator(item, *(current -> key)))
        {
            return;
        }
        //std::cout << "skip list insertion : " << item << std::endl;
        int lvl = randomLevel();
        if(lvl > level)
        {
            for(int i = level + 1; i < lvl + 1; i++)
            {
                update[i] = header;
            }
            level = lvl;
        }
        SkipListNode * newNode = createNode(item, lvl);
        for(int i = 0; i <= lvl; i++)
        {
            newNode -> forward[i] = update[i] -> forward[i];
            update[i] -> forward[i] = newNode;
        }
    }

    // Search for a value in the skip list
    bool search(const T & item)
    {
        SkipListNode * current = header;
        int i;
        for(i = level; i >= 0; i--)
        {
            while(current -> forward[i] != nullptr && comparator(*(current -> forward[i] -> key), item))
            {
                current = current -> forward[i];
            }
            if(current -> forward[i] != nullptr && !comparator(item, *(current -> forward[i] -> key)) && !comparator(*(current -> forward[i] -> key), item))
            {
                return true;
            }
        }
        return false;
    }

    // Delete a value from the skip list
    void deleteItem(const T & item)
    {
        std::vector<SkipListNode *> update(MAXLVL + 1, nullptr);
        SkipListNode * current = header;
        for(int i = level; i >= 0; i--)
        {
            while(current -> forward[i] != nullptr && comparator(*(current -> forward[i] -> key), item))
            {
                current = current -> forward[i];
            }
            update[i] = current;
        }
        current = current -> forward[0];
        if(current != nullptr && !comparator(item, *(current -> key)) && !comparator(*(current -> key), item))
        {
            //std::cout << "skip list deletion : " << item << std::endl;
            for(int i = 0; i <= level; i++)
            {
                if(update[i] -> forward[i] != current)
                {
                    break;
                }
                update[i] -> forward[i] = current -> forward[i];
            }
            delete current;
            while(level > 0 && header -> forward[level] == nullptr)
            {
                level--;
            }
        }
    }
};

#endif