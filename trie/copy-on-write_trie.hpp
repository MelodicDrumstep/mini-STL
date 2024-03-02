#pragma once

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
    this->is_value_node_ = true;
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
  //throw NotImplementedException("Trie::Get is not implemented.");

    if(key.size() == 0)
    {
        return nullptr;
    }

    size_t i = 0;
    std::shared_ptr<const TrieNode> cur = root_;
    
    while(i < key.size() && cur -> children_.find(key[i]) != cur -> children_.end()) 
    //This means that the current node has a child with the key[i]
    {
        cur = cur -> children_.at(key[i]);
        i++;
    }
    if(i == key.size())
    {
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


    size_t i = 0;
    std::shared_ptr<const TrieNode> old_cur = root_;
    
    std::shared_ptr<TrieNode> new_root;

    if(root_ == nullptr)
    {
        ////std::cout << "root is null" << std::endl;
        new_root = std::make_shared<TrieNode>();
        old_cur = new_root;
    }
    else
    {
        new_root = root_ -> Clone();
    }

    ////std::cou << "! " << std::endl;

    std::shared_ptr<TrieNode> new_cur = new_root;

    //std::cout << "key size : " << key.size() << std::endl;
    
    //std::cout << " find : " << (old_cur -> children_.find(key[i]) != old_cur -> children_.end()) << std::endl;

    while(i < key.size() && old_cur -> children_.find(key[i]) != old_cur -> children_.end())
    {
        //std::cout << "!!!" << std::endl;
        if(i == key.size() - 1)
        {
        std::shared_ptr<T> ptr2value = std::make_shared<T>(std::move(value));
        std::shared_ptr<TrieNodeWithValue<T>> temp = std::make_shared<TrieNodeWithValue<T>>(ptr2value);
        new_cur -> children_[key[i]] = temp;
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

