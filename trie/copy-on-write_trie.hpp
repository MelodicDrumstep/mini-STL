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
	auto temp_node = root_;
  //Now this assignment operator is "move constructor"
  //It will just let temp_node be a shared_ptr pointing to the root node

  if(!temp_node)
  {
    return nullptr;
  }

  for(char c : key)
  {
    if(!temp_node || temp_node -> children_.find(c) == temp_node -> children_.end())
    {
      //This means we can not find the key
      return nullptr;
    }
    //go down. this is move assignment 
    temp_node = temp_node -> children_.at(c);
  }

  if(temp_node -> is_value_node_)
  {
    //Notice: 1.We can only use dynamic_cast to origin pointer or reference type
    //Cannot use it to the std::shared_ptr
    //Now I have to dynamic_cast the ptr to "TrieNodeWithValue * ". 
    //So as to interprete the node as "TrieNodeWithValue" and use its "value_" field.
    auto value_node_ptr = dynamic_cast<const TrieNodeWithValue<T> *>(temp_node.get());
    if(!value_node_ptr)
    {
      return nullptr;
    }
    return value_node_ptr -> value_.get();
    //value_ is std::shared_ptr<T>
  }

  return nullptr;
}

  // Put a new key-value pair into the trie. If the key already exists, overwrite the value.
  // Returns the new trie.
template <class T>
auto Put(std::string_view key, T value) const -> Trie 
{
  // Note that `T` might be a non-copyable type. Always use `std::move` when
  // creating `shared_ptr` on that value. throw
  // NotImplementedException("Trie::Put is not implemented.");

  std::shared_ptr<TrieNode> temp_node;

  if(!root_)
  {
    temp_node = std::make_shared<TrieNode>();
    //The trie is empty. Create a new node
  }
  else
  {
    temp_node = std::shared_ptr<TrieNode>(std::move(root_ -> Clone()));
  }
  //otherwise, Clone the root
  //Clone() just std::make_unique to DEEP COPY a node. 
  //I can use std::shared_ptr<TrieNode>(std::move(root_ -> Clone()))
  //to convert it into std::shared_ptr

  std::stack<std::shared_ptr<TrieNode>> stk;
  //Use a stack to build the new trie from the bottom to the top.

  for(auto c : key)
  {
    stk.push(temp_node);
    if(!temp_node || temp_node -> children_.find(c) == temp_node -> children_.end())
    {
      temp_node = std::make_shared<TrieNode>();
    }
    else
    {
      temp_node = std::shared_ptr<TrieNode>(std::move(temp_node -> children_.at(c) -> Clone()));
    }
  }

/*
  Warning:
  dynamic_pointer_cast can only convert Derived to Base.
  It cannot convert Base to Derived.
  When I try to convert a shared_ptr<TrieNodeWithValue<T>> to shared_ptr<TrieNode>
  it's always ok
  However, when I try to convert a shared_ptr<TrieNode> to shared_ptr<TrieNodeWithValue<T>>
  It only works when the original pointer is pointing to a TrieNodeWithValue<T> object
  Otherwise, it will return nullptr
  So the following code will not work
*/

  // auto new_temp_node = std::dynamic_pointer_cast<TrieNodeWithValue<T>>(temp_node);
  // new_temp_node -> is_value_node_ = 1;
  // new_temp_node -> value_ = std::make_shared<T>(std::move(value));
  // temp_node = std::dynamic_pointer_cast<TrieNode>(new_temp_node);


  // This is the right way to do it:
  // When I have to convert a Base object to a Derived object
  // I have to CREATE a new Derived object using the Base object
  // rather than cast the pointer
  // Below shows how to do it: std::make_shared<TrieNodeWithValue<T>>(...) will CREATE a new TrieNodeWithValue object
  //Then I command temp_node points to it
  // (Base pointer can point to a Derived object, but it cannot access the Derived object's member)
  temp_node = std::make_shared<TrieNodeWithValue<T>>(temp_node -> children_, std::make_shared<T>(std::move(value)));

  for(int k = key.size() - 1; k >= 0; k--)
  {
    char c = key[k];
    stk.top() -> children_[c] = temp_node;
    temp_node = stk.top();
    stk.pop();
  }

  return Trie(temp_node);
  // You should walk through the trie and create new nodes if necessary. If the
  // node corresponding to the key already exists, you should create a new
  // `TrieNodeWithValue`.
}

  // Remove the key from the trie. If the key does not exist, return the original trie.
  // Otherwise, returns the new trie.
auto Remove(std::string_view key) const -> Trie
{
  if(!root_)
  {
    return Trie();
  }

	#ifdef DEBUG
	std::cout << "Remove() is called!" << std::endl;
	std::cout << "key is " << key << std::endl;
	std::cout << "key size is " << key.size() << std::endl;
	std::cout << "And root has value? " << root_ -> is_value_node_ << std::endl;
	#endif

	bool temp_bool = root_ -> is_value_node_;
  std::shared_ptr<TrieNode> temp_node = std::shared_ptr<TrieNode>(std::move(root_ -> Clone()));
	temp_node -> is_value_node_ = temp_bool;

	#ifdef DEBUG
	std::cout << "temp_node has value? " << temp_node -> is_value_node_ << std::endl;
	#endif

  if(key.size() == 0)
  {
    #ifdef DEBUG
    std::cout << "the keysize is 0! " << std::endl << "And the is_value_node : " << temp_node -> is_value_node_ << std::endl; 
    #endif

    if(!temp_node -> is_value_node_)
    {
      return *this;
    }
    else
    {
      temp_node = std::make_shared<TrieNode>(temp_node -> children_);
      return Trie(temp_node);
    }
  }

  std::stack<std::shared_ptr<TrieNode>> stk;
  //Use a stack to build the new trie from the bottom to the top.

  for(auto c : key)
  {
    stk.push(temp_node);
    if(!temp_node || temp_node -> children_.find(c) == temp_node -> children_.end())
    {
      temp_node = nullptr;
      break;
    }
    else
    {
      temp_node = std::shared_ptr<TrieNode>(std::move(temp_node -> children_.at(c) -> Clone()));
    }
  }

if(!temp_node || !temp_node -> is_value_node_)
  {
    return Trie(root_);
  }
  else
  {
    if(temp_node -> children_.empty())
    {
      temp_node = nullptr;
	  //stk.top() -> children_.erase(key[key.size() - 1]);
    }
    else
    {
      temp_node = std::make_shared<TrieNode>(temp_node -> children_);
    }
  }

  for(int k = key.size() - 1; k >= 0; k--)
  {
    char c = key[k];
    if(temp_node == nullptr)
    {
      stk.top() -> children_.erase(c);
    }
	  else
    {
      stk.top() -> children_[c] = temp_node;
    }
    temp_node = stk.top();

	  if(!temp_node -> is_value_node_ && temp_node -> children_.empty())
    {
      temp_node = nullptr;
	  //stk.top() -> children_.erase(key[key.size() - 1]);
    }
    stk.pop();
  }

  return Trie(temp_node);
}

  // Get the root of the trie, should only be used in test cases.
  auto GetRoot() const -> std::shared_ptr<const TrieNode> { return root_; }
};

