#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace shevchenko
{
template< class Key, class Value >
class BSTIterator;

template< class Key, class Value >
class BSTConstIterator;

template< class Key, class Value, class Compare = std::less< Key > >
class BSTree
{
private:
  struct Node
  {
    std::pair< Key, Value > data;
    
    Node* parent;
    Node* left;
    Node* right;
    
    Node(const Key& key, const Value& value, Node* par):
    data(key, value),
    parent(par),
    left(nullptr),
    right(nullptr)
    {
    }
  };
  
public:
  using iterator = BSTIterator< Key, Value >;
  using const_iterator = BSTConstIterator< Key, Value >;
  
  BSTree();
  BSTree(const BSTree& other);
  BSTree(BSTree&& other) noexcept;
  ~BSTree();
  BSTree& operator=(const BSTree& other);
  BSTree& operator=(BSTree&& other) noexcept;
  
  void push(const Key& key, const Value& value);
  Value get(const Key& key) const;
  Value drop(const Key& key);
  
  bool empty() const noexcept;
  
  size_t size() const noexcept;
  void clear();
  
  iterator begin();
  iterator end();
  
  const_iterator cbegin() const;
  const_iterator cend() const;
  
  const_iterator rotateLeft(const_iterator it);
  const_iterator rotateRight(const_iterator it);
  
  const_iterator rotateLargeLeft(const_iterator it);
  const_iterator rotateLargeRight(const_iterator it);
  
  size_t height() const;
  size_t height(const_iterator it) const;
  
private:
  Node* fake_;
  size_t size_;
  Compare comp_;
  
  void clear(Node* node);
  
  Node* copy(Node* other, Node* parent);
  Node* minimum(Node* node) const;
  Node* findNode(const Key& key) const;
  
  size_t height(Node* node) const;
};

template< class Key, class Value >
class BSTConstIterator
{
  template< class K, class V, class C >
  friend class BSTree;
  
public:
  BSTConstIterator():
  node_(nullptr),
  fake_(nullptr)
  {}
  
  const std::pair< Key, Value >& operator*() const
  {
    return node_->data;
  }
  
  const std::pair< Key, Value >* operator->() const
  {
    return std::addressof(node_->data);
  }
  
  BSTConstIterator& operator++()
  {
    if (node_->right != nullptr)
    {
      node_ = node_->right;
      while (node_->left != nullptr)
      {
        node_ = node_->left;
      }
      
      return *this;
    }
    Node* parent = node_->parent;
    
    while ((parent != fake_) && (node_ == parent->right))
    {
      node_ = parent;
      parent = parent->parent;
    }
    node_ = parent;

    return *this;
  }
  
  BSTConstIterator operator++(int)
  {
    BSTConstIterator tmp(*this);
    ++(*this);
    return tmp;
  }
  
  bool operator==(const BSTConstIterator& other) const
  {
    return node_ == other.node_;
  }
  
  bool operator!=(const BSTConstIterator& other) const
  {
    return !(*this == other);
  }
  
private:
  struct Node;
  
  const Node* node_;
  const Node* fake_;
  
  BSTConstIterator(const Node* node, const Node* fake):
  node_(node),
  fake_(fake)
  {}
};

template< class Key, class Value >
class BSTIterator
{
  template< class K, class V, class C >
  friend class BSTree;
  
public:
  BSTIterator():
  node_(nullptr),
  fake_(nullptr)
  {}
  
  operator BSTConstIterator< Key, Value >() const
  {
    return BSTConstIterator< Key, Value >(node_, fake_);
  }
  
  std::pair< Key, Value >& operator*() const
  {
    return node_->data;
  }
  
  std::pair< Key, Value >* operator->() const
  {
    return std::addressof(node_->data);
  }
  
  BSTIterator& operator++()
  {
    if (node_->right != nullptr)
    {
      node_ = node_->right;
      while (node_->left != nullptr)
      {
        node_ = node_->left;
      }
      
      return *this;
    }
    
    Node* parent = node_->parent;
    while ((parent != fake_) && (node_ == parent->right))
    {
      node_ = parent;
      parent = parent->parent;
    }
    
    node_ = parent;
    
    return *this;
  }
  
  BSTIterator operator++(int)
  {
    BSTIterator tmp(*this);
    ++(*this);
    return tmp;
  }
  
  bool operator==(const BSTIterator& other) const
  {
    return node_ == other.node_;
  }
  
  bool operator!=(const BSTIterator& other) const
  {
    return !(*this == other);
  }
  
private:
  struct Node;
  
  Node* node_;
  Node* fake_;
  
  BSTIterator(Node* node, Node* fake):
  node_(node),
  fake_(fake)
  {}
};

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::BSTree():
fake_(new Node(Key(), Value(), nullptr)),
size_(0),
comp_(Compare())
{
}

template< class Key, class Value, class Compare >
BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
  delete fake_;
}

template< class Key, class Value, class Compare >
bool BSTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
size_t BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::clear()
{
  clear(fake_->left);
  fake_->left = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::clear(Node* node)
{
  if (node == nullptr)
  {
    return;
  }
  
  clear(node->left);
  clear(node->right);
  
  delete node;
}

template< class Key, class Value, class Compare >
void BSTree< Key, Value, Compare >::push(const Key& key, const Value& value)
{
  if (fake_->left == nullptr)
  {
    fake_->left = new Node(key, value, fake_);
    ++size_;
    return;
  }
  
  Node* current = fake_->left;
  Node* parent = nullptr;
  
  while (current != nullptr)
  {
    parent = current;
    
    if (comp_(key, current->data.first))
    {
      current = current->left;
    }
    else if (comp_(current->data.first, key))
    {
      current = current->right;
    }
    else
    {
      current->data.second = value;
      return;
    }
  }
  
  Node* node = new Node(key, value, parent);
  
  if (comp_(key, parent->data.first))
  {
    parent->left = node;
  }
  else
  {
    parent->right = node;
  }
  
  ++size_;
}

template< class Key, class Value, class Compare >
Value BSTree< Key, Value, Compare >::get(const Key& key) const
{
  Node* current = fake_->left;
  
  while (current != nullptr)
  {
    if (comp_(key, current->data.first))
    {
      current = current->left;
    }
    else if (comp_(current->data.first, key))
    {
      current = current->right;
    }
    else
    {
      return current->data.second;
    }
  }
  
  throw std::out_of_range("no such key");
}
