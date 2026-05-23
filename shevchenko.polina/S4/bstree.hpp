#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
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
