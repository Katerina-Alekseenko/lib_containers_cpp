#ifndef LIB_SET_H_
#define LIB_SET_H_

#include "lib_tree.h"

namespace lib {
template <typename Key>
class set {
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using BinaryTree = RBTree<value_type>;
  using size_type = std::size_t;

 public:
  using iterator = typename BinaryTree::iterator;
  using const_iterator = typename BinaryTree::const_iterator;

  set() : rbtree_() {}

  set(std::initializer_list<value_type> const& items) {
    for (auto it : items) {
      rbtree_.insertUnique(it);
    }
  }

  set(const set& s) : rbtree_(s.rbtree_) {}
  set(set&& s) : rbtree_(std::move(s.rbtree_)) {}
  ~set() = default;

  set& operator=(const set& s) {
    rbtree_ = s.rbtree_;
    return *this;
  }

  set& operator=(set&& s) {
    rbtree_ = std::move(s.rbtree_);
    return *this;
  }

  iterator begin() noexcept { return rbtree_.begin(); }
  iterator end() noexcept { return rbtree_.end(); }
  const_iterator begin() const noexcept { return rbtree_.begin(); }
  const_iterator end() const noexcept { return rbtree_.end(); }

  bool empty() const noexcept { return rbtree_.empty(); }
  size_type size() const noexcept { return rbtree_.size(); }
  size_type max_size() const noexcept { return rbtree_.max_size(); }

  void clear() { rbtree_.clear(); }

  std::pair<iterator, bool> insert(const_reference key) {
    return rbtree_.insertUnique(key);
  }

  void erase(iterator pos) { rbtree_.erase(pos); }
  void swap(set& other) { rbtree_.swap(other.rbtree_); }
  void merge(set& other) noexcept { rbtree_.mergeUnique(other.rbtree_); }

  iterator find(const_reference key) { return rbtree_.find(key); }
  const_iterator find(const_reference key) const { return rbtree_.find(key); }
  bool contains(const_reference key) const { return rbtree_.contains(key); }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    return rbtree_.insertMany(std::forward<Args>(args)...);
  }

 private:
  BinaryTree rbtree_;
};
}  // namespace lib

#endif  // LIB_SET_H_
