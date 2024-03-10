#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <algorithm>
#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string_view>
#include <iostream>
#include <shared_mutex>
#include <string_view>
#include <mutex>  
#include <future> 
//#include "../utility/print_utility.hpp"

namespace sjtu
{
/// A special type that will block the move constructor and move assignment operator. Used in TrieStore tests.
class MoveBlocked {
 public:
  explicit MoveBlocked(std::future<int> wait) : wait_(std::move(wait)) {}

  MoveBlocked(const MoveBlocked &) = delete;
  MoveBlocked(MoveBlocked &&that) noexcept {
    if (!that.waited_) {
      that.wait_.get();
    }
    that.waited_ = waited_ = true;
  }

  auto operator=(const MoveBlocked &) -> MoveBlocked & = delete;
  auto operator=(MoveBlocked &&that) noexcept -> MoveBlocked & {
    if (!that.waited_) {
      that.wait_.get();
    }
    that.waited_ = waited_ = true;
    return *this;
  }

  bool waited_{false};
  std::future<int> wait_;
};

// A TrieNode is a node in a Trie.
class TrieNode 
{
 public:
  // Create a TrieNode with no children.
  TrieNode() = default;

  // Create a TrieNode with some children.
  explicit TrieNode(std::map<char, std::shared_ptr<const TrieNode>> children) : children_(std::move(children)) {}

  virtual ~TrieNode() = default;

  // Clone returns a copy of this TrieNode. If the TrieNode has a value, the value is copied. The return
  // type of this function is a unique_ptr to a TrieNode.
  //
  // cannot use the copy constructor to clone the node because it doesn't know whether a `TrieNode`
  // contains a value or not.
  //
  //convert `unique_ptr` into `shared_ptr`: use `std::shared_ptr<T>(std::move(ptr))`.
  virtual auto Clone() const -> std::unique_ptr<TrieNode> { return std::make_unique<TrieNode>(children_); }

  // A map of children, where the key is the next character in the key, and the value is the next TrieNode.

  std::map<char, std::shared_ptr<const TrieNode>> children_;

  // Indicates if the node is the terminal node.
  bool is_value_node_{false};

};

// A TrieNodeWithValue is a TrieNode that also has a value of type T associated with it.
template <class T>
class TrieNodeWithValue : public TrieNode {
 public:
  // Create a trie node with no children and a value.
  explicit TrieNodeWithValue(std::shared_ptr<T> value) : value_(std::move(value)) { this->is_value_node_ = true; }

  // Create a trie node with children and a value.
  TrieNodeWithValue(std::map<char, std::shared_ptr<const TrieNode>> children, std::shared_ptr<T> value)
      : TrieNode(std::move(children)), value_(std::move(value)) {
    this -> is_value_node_ = true;
  }

  // Override the Clone method to also clone the value.
  //
  // Nconvert `unique_ptr` into `shared_ptr` : use `std::shared_ptr<T>(std::move(ptr))`.
  auto Clone() const -> std::unique_ptr<TrieNode> override {
    return std::make_unique<TrieNodeWithValue<T>>(children_, value_);
  }

  // The value associated with this trie node.
  std::shared_ptr<T> value_;
};

// A Trie is a data structure that maps strings to values of type T. All operations on a Trie should not
// modify the trie itself. It should reuse the existing nodes as much as possible, and create new nodes to
// represent the new trie.
//

class Trie 
{
 private:
  // The root of the trie.
  std::shared_ptr<const TrieNode> root_{nullptr};

  // Create a new trie with the given root.
  explicit Trie(std::shared_ptr<const TrieNode> root) : root_(std::move(root)) {}

 public:
  // Create an empty trie.
  Trie() = default;

  // Get the value associated with the given key.
  // 1. If the key is not in the trie, return nullptr.
  // 2. If the key is in the trie but the type is mismatched, return nullptr.
  // 3. Otherwise, return the value.
  template <class T>
  auto Get(std::string_view key) const -> const T * 
  {
    //std::cout << "Enter function Get" << std::endl;
  //throw NotImplementedException("Trie::Get is not implemented.");

    //std::cout << "Now i'm getting key : " << key << std::endl;
    //std::cout << "Key size is : " << key.size() << std::endl;

    if(root_ == nullptr)
    {
        return nullptr;
    }

    if(key.size() == 0)
    {
        return nullptr;
    }

    size_t i = 0;
    std::shared_ptr<const TrieNode> cur = root_;
    
    while(i < key.size() && cur -> children_.find(key[i]) != cur -> children_.end()) 
    //This means that the current node has a child with the key[i]
    {
        //std::cout << "i is : " << i << std::endl;
        //std::cout << "key[i] is : " << key[i] << std::endl;
        cur = cur -> children_.at(key[i]);
        i++;
    }
    //std::cout << " after the loop, i is : " << i << std::endl;
    if(i == key.size())
    {
        //std::cout << "cur is pointing at a value node or not :  " << (*cur).is_value_node_ << std::endl;
        const TrieNodeWithValue<T>* node = dynamic_cast<const TrieNodeWithValue<T>*>(cur.get());
        if(node != nullptr) 
        {
        return ((node -> value_).get());
        } 
        else 
        {
        return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
  }

  // Put a new key-value pair into the trie. If the key already exists, overwrite the value.
  // Returns the new trie.
  template <class T>
  auto Put(std::string_view key, T value) const -> Trie 
  {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  //throw NotImplementedException("Trie::Put is not implemented.");


//DEBUG
    ////std::cout << "Enter function Put" << std::endl;
    ////std::cout << "I'm currently putting : " << key << " -> " << value << std::endl;

    cnt1++;

//DEBUG

    size_t i = 0;
    std::shared_ptr<const TrieNode> old_cur = root_;
    
    std::shared_ptr<TrieNode> new_root;

    if(root_ == nullptr)
    {
        ////////std::cout << "root is null" << std::endl;
        new_root = std::make_shared<TrieNode>();
        old_cur = new_root;
    }
    else
    {
        new_root = root_ -> Clone();
    }

    ////std::cou << "! " << std::endl;

    std::shared_ptr<TrieNode> new_cur = new_root;

    ////std::cout << "key size : " << key.size() << std::endl;
    
    ////std::cout << " find : " << (old_cur -> children_.find(key[i]) != old_cur -> children_.end()) << std::endl;

    while(i < key.size() && old_cur -> children_.find(key[i]) != old_cur -> children_.end())
    {
        //std::cout << "i is : " << i << std::endl;
        //std::cout << "key[i] is : " << key[i] << std::endl;
        ////std::cout << "!!!" << std::endl;
        if(i == key.size() - 1)
        {
            //std::cout << "Right here" << std::endl;

            //std::cout << "newcur : " << new_cur -> children_.size() << std::endl;
            //std::cout << "oldcur : " << old_cur -> children_.size() << std::endl;
            for(auto i : old_cur -> children_)
            {
                //std::cout << "the chilren : " << i.first << " -> " << i.second << std::endl;
            }

            //std::shared_ptr<T> ptr2value = std::make_shared<T>(std::move(value));
            
            std::shared_ptr<TrieNode> temp = old_cur -> children_.at(key[i]) -> Clone();

            //std::cout << "temp : " << temp -> children_.size() << std::endl;

            //auto tempWithValue = dynamic_cast<TrieNodeWithValue<T>>(temp.get());

            std::shared_ptr<TrieNodeWithValue<T>> tempwithValue = std::make_shared<TrieNodeWithValue<T>>(temp -> children_, std::make_shared<T>(std::move(value)));


            // //std::cout << "tempWithValue : " << tempWithValue << std::endl;

            // tempWithValue -> is_value_node_ = true;
            // tempWithValue -> value_ = std::make_shared<T>(std::move(value));
            new_cur -> children_[key[i]] = tempwithValue;

            //std::shared_ptr<TrieNode> temp = old_cur -> Clone();
            // std::shared_ptr<T> ptr2value = std::makeshared<T>(std::move(value));
            // std::shared_ptr<TrieNodeWithValue<T>> temp = std::make_shared<TrieNodeWithValue<T>>(ptr2value);
            // new_cur -> children_[key[i]] = temp;
        }
        else
        {

            std::shared_ptr<TrieNode> temp = old_cur -> children_.at(key[i]) -> Clone();
            new_cur -> children_[key[i]] = temp;
            new_cur = temp;
            old_cur = old_cur -> children_.at(key[i]);
        }
        i++;
    }
    

    while(i < key.size())
    {
        //std::cout << "In the second while loop, i is  : " << i << std::endl;
        if(i == key.size() - 1)
        {
        std::shared_ptr<T> ptr2value = std::make_shared<T>(std::move(value));
        std::shared_ptr<TrieNodeWithValue<T>> temp = std::make_shared<TrieNodeWithValue<T>>(ptr2value);
        new_cur -> children_[key[i]] = temp;
        }
        else
        {
        std::shared_ptr<TrieNode> temp = std::make_shared<TrieNode>();
        new_cur -> children_[key[i]] = temp;
        new_cur = temp;
        }
        i++;
    }
    
    return Trie(new_root);

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
}

  // Remove the key from the trie. If the key does not exist, return the original trie.
  // Otherwise, returns the new trie.
  auto Remove(std::string_view key) const -> Trie
  {
    
    if(key.size() == 0)
    {
        return *this;
    }

    size_t i = 0;
    std::shared_ptr<const TrieNode> cur = root_;
    std::shared_ptr<TrieNode> new_root = root_ -> Clone();
    std::shared_ptr<TrieNode> new_cur = new_root;

    while(i < key.size() && cur -> children_.find(key[i]) != cur -> children_.end())
    {
        if(i == key.size() - 1)
        {
            if(cur -> children_.at(key[i]) -> is_value_node_)
            {
                if(cur -> children_.at(key[i]) -> children_.size() == 0)
                {
                    new_cur -> children_.erase(key[i]);
                }
                else
                {
                   std::shared_ptr<TrieNode> temp = std::make_shared<TrieNode>(std::move(cur -> children_.at(key[i]) -> children_));
                     new_cur -> children_[key[i]] = temp;
                }
            }
        }
        else
        {
        std::shared_ptr<TrieNode> temp = cur -> children_.at(key[i]) -> Clone();
        new_cur -> children_[key[i]] = temp;
        new_cur = temp;
        cur = cur -> children_.at(key[i]);
        }
        i++;
    }
    return Trie(new_root);
  }

  // Get the root of the trie, should only be used in test cases.
  auto GetRoot() const -> std::shared_ptr<const TrieNode> { return root_; }
};

template <class T>
class ValueGuard {
 public:
  ValueGuard(Trie root, const T &value) : root_(std::move(root)), value_(value) {}
  auto operator*() const -> const T & { return value_; }

 private:
  Trie root_;
  const T &value_;
};


// This class is a thread-safe wrapper around the Trie class. It provides a simple interface for
// accessing the trie. It should allow concurrent reads and a single write operation at the same
// time.
class TrieStore {
 public:
  // This function returns a ValueGuard object that holds a reference to the value in the trie. If
  // the key does not exist in the trie, it will return std::nullopt.
  template <class T>
  auto Get(std::string_view key) -> std::optional<ValueGuard<T>>
  {
    // Pseudo-code:
    // (1) Take the root lock, get the root, and release the root lock. Don't lookup the value in the
    //     trie while holding the root lock.
    // (2) Lookup the value in the trie.
    // (3) If the value is found, return a ValueGuard object that holds a reference to the value and the
    //     root. Otherwise, return std::nullopt.
    //throw NotImplementedException("TrieStore::Get is not implemented.");
    root_lock_.lock();
    Trie & cur = root_;
    root_lock_.unlock();
    const T * res = cur.Get<T>(key);
    if (res == nullptr) 
    {
        return std::nullopt;
    }
    return ValueGuard<T>(cur, *(res));
  }
  
  // This function will insert the key-value pair into the trie. If the key already exists in the
  // trie, it will overwrite the value.
  template <class T>
  void Put(std::string_view key, T value)
  {
    write_lock_.lock();
    root_lock_.lock();
    root_ = root_.Put<T>(key, std::move(value));
    root_lock_.unlock();
    write_lock_.unlock();
  }

  // This function will remove the key-value pair from the trie.
  void Remove(std::string_view key)
  {
    // You will need to ensure there is only one writer at a time. Think of how you can achieve this.
    // The logic should be somehow similar to `TrieStore::Get`.
    //throw NotImplementedException("TrieStore::Remove is not implemented.");
    write_lock_.lock();
    root_lock_.lock();
    root_ = root_.Remove(key);
    root_lock_.unlock();
    write_lock_.unlock();
  }

 private:
  // This mutex protects the root. Every time you want to access the trie root or modify it, you
  // will need to take this lock.
  std::mutex root_lock_;

  // This mutex sequences all writes operations and allows only one write operation at a time.
  std::mutex write_lock_;

  // Stores the current root for the trie.
  Trie root_;
};

}

#endif  // SOLUTION_HPP

