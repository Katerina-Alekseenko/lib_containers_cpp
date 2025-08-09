#ifndef LIB_MAP_H_
#define LIB_MAP_H_

#include "lib_tree.h"

namespace lib {
template <typename Key, typename T>
class map {
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using BinaryTree = RBTree<value_type>;
  using size_type = std::size_t;

 public:
  using iterator = typename BinaryTree::iterator;
  using const_iterator = typename BinaryTree::const_iterator;

  map() : rbtree_() {}

  map(std::initializer_list<value_type> const& items) {
    for (auto it : items) {
      rbtree_.insertUnique(it);
    }
  }

  map(const map& m) : rbtree_(m.rbtree_) {}
  map(map&& m) : rbtree_(std::move(m.rbtree_)) {}
  ~map() = default;

  map& operator=(map& m) {
    rbtree_ = m.rbtree_;
    return *this;
  }

  map& operator=(map&& m) {
    rbtree_ = std::move(m.rbtree_);
    return *this;
  }

  T& at(const Key& key) {
    iterator it = findKey_(key);
    if (it == rbtree_.end()) throw std::out_of_range("Key not found");
    return (*it).second;
  }

  const T& at(const Key& key) const {
    const_iterator it = findKey_(key);
    if (it == rbtree_.end()) throw std::out_of_range("Key not found");
    return (*it).second;
  }

  T& operator[](const Key& key) {
    value_type value = {key, mapped_type{}};
    iterator it = findKey_(key);
    if (it == rbtree_.end()) {
      auto res = rbtree_.insertUnique(value);
      return (*res.first).second;
    }
    return (*it).second;
  }

  iterator begin() noexcept { return rbtree_.begin(); }
  iterator end() noexcept { return rbtree_.end(); }
  const_iterator begin() const noexcept { return rbtree_.begin(); }
  const_iterator end() const noexcept { return rbtree_.end(); }

  bool empty() const noexcept { return rbtree_.empty(); }
  size_type size() const noexcept { return rbtree_.size(); }
  size_type max_size() const noexcept { return rbtree_.max_size(); }

  void clear() { rbtree_.clear(); }

  std::pair<iterator, bool> insert(const value_type& value) {
    iterator it = findKey_(value.first);
    if (it == end()) {
      auto res = rbtree_.insertUnique(value);
      return res;
    }
    return {it, false};
  }

  std::pair<iterator, bool> insert(const Key& key, const T& obj) {
    value_type value = {key, obj};
    iterator it = findKey_(value.first);
    if (it == end()) {
      auto res = rbtree_.insertUnique(value);
      return res;
    }
    return {it, false};
  }

  std::pair<iterator, bool> insert_or_assign(const Key& key, const T& obj) {
    value_type value = {key, obj};
    iterator it = findKey_(key);
    if (it == rbtree_.end())
      return rbtree_.insertUnique(value);
    else {
      (*it).second = obj;
      return {it, false};
    }
  }

  void erase(iterator pos) {
    auto it = findKey_((*pos).first);
    if (it != end()) rbtree_.erase(pos);
  }

  void swap(map& other) { rbtree_.swap(other.rbtree_); }
  void merge(map& other) { rbtree_.mergeUnique(other.rbtree_); }

  bool contains(const Key& key) const noexcept {
    const_iterator it = findKey_(key);
    return it != end();
  }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args) {
    return rbtree_.insertMany(std::forward<Args>(args)...);
  }

 private:
  iterator findKey_(const Key& key) noexcept {
    iterator it = rbtree_.begin();
    if (it == rbtree_.end() || it == nullptr) return end();
    while (it != rbtree_.end()) {
      if ((*it).first == key) return it;
      it++;
    }
    return end();
  }

  const_iterator findKey_(const Key& key) const noexcept {
    const_iterator it = rbtree_.begin();
    if (it == rbtree_.end()) return end();
    while (it != rbtree_.end()) {
      if ((*it).first == key) return it;
      it++;
    }
    return end();
  }

  BinaryTree rbtree_;
};
}  // namespace lib

#endif  // SRC_LIB_MAP_H_
