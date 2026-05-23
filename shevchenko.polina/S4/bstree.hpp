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

};
