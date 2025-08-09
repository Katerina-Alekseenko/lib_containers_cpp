#ifndef LIB_MULTISET_H
#define LIB_MULTISET_H

#include "lib_tree.h"

namespace lib {
template <typename Key>
class multiset {
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using BinaryTree = RBTree<value_type>;
  using size_type = std::size_t;

 public:
  using iterator = typename BinaryTree::iterator;
  using const_iterator = typename BinaryTree::const_iterator;

  multiset() : rbtree_() {}

  multiset(std::initializer_list<value_type> const& items) {
    for (auto it : items) {
      rbtree_.insertDuplicate(it);
    }
  }

  multiset(const multiset& ms) : rbtree_(ms.rbtree_) {}
  multiset(multiset&& ms) : rbtree_(std::move(ms.rbtree_)) {}
  ~multiset() = default;

  multiset& operator=(const multiset& ms) {
    rbtree_ = ms.rbtree_;
    return *this;
  }

  multiset& operator=(multiset&& ms) {
    rbtree_ = std::move(ms.rbtree_);
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
  iterator insert(const_reference key) { return rbtree_.insertDuplicate(key); }
  void erase(iterator pos) { rbtree_.erase(pos); }
  void swap(multiset& other) { rbtree_.swap(other.rbtree_); }
  void merge(multiset& other) { rbtree_.mergeDuplicates(other.rbtree_); }

  size_type count(const_reference key) const noexcept {
    return rbtree_.count(key);
  }

  iterator find(const_reference key) noexcept { return rbtree_.find(key); }

  const_iterator find(const_reference key) const noexcept {
    return rbtree_.find(key);
  }

  bool contains(const_reference key) const noexcept {
    return rbtree_.contains(key);
  }

  std::pair<iterator, iterator> equal_range(const_reference key) noexcept {
    return rbtree_.equal_range(key);
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const_reference key) const noexcept {
    return rbtree_.equal_range(key);
  }

  iterator lower_bound(const_reference key) noexcept {
    return rbtree_.lower_bound(key);
  }

  iterator upper_bound(const_reference key) noexcept {
    return rbtree_.upper_bound(key);
  }

  const_iterator lower_bound(const_reference key) const noexcept {
    return rbtree_.lower_bound(key);
  }

  const_iterator upper_bound(const_reference key) const noexcept {
    return rbtree_.upper_bound(key);
  }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    return rbtree_.insertManyDuplicate(std::forward<Args>(args)...);
  }

 private:
  BinaryTree rbtree_;
};
}  // namespace lib

#endif  // LIB_MULTISET_H
