#ifndef LIB_LIST_H_
#define LIB_LIST_H_

#include <cstddef>
#include <limits>
#include <utility>

namespace lib {
template <typename T>
class list {
 public:
  class ListIterator;
  class ListConstIterator;

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using iterator = ListIterator;
  using const_iterator = ListConstIterator;
  using size_type = size_t;

  list();
  explicit list(size_type n);
  list(std::initializer_list<value_type> const& items);
  list(const list& l);
  list(list&& l);
  ~list();
  list& operator=(list&& l);

  const_reference front() const;
  const_reference back() const;

  iterator begin();
  const_iterator cbegin() const;
  iterator end();
  const_iterator cend() const;

  bool empty() const;
  size_type size() const;
  size_type max_size() const;

  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void push_front(const_reference value);
  void pop_front();
  void swap(list& other);
  void merge(list& other);
  void splice(const_iterator pos, list& other);
  void reverse();
  void unique();
  void sort();

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args&&... args);

  template <typename... Args>
  void insert_many_back(Args&&... args);

  template <typename... Args>
  void insert_many_front(Args&&... args);

 private:
  struct Node;

  Node* head_;
  Node* tail_;
  size_type size_;

  void deallocate();
  Node* detachNode(iterator pos);
  Node* detachNode(Node* node);
  void eraseNode(Node* node);
  iterator insertNode(iterator pos, Node* node);
  list splitForSort();
};

template <typename T>
class list<T>::ListIterator {
  friend class list<T>;

 public:
  ListIterator() = default;
  ListIterator(const ListIterator& li);

  reference operator*();

  ListIterator& operator++();
  ListIterator operator++(int);

  ListIterator& operator--();
  ListIterator operator--(int);

  ListIterator& operator=(const ListIterator& li) = default;
  bool operator==(const ListIterator& li) const;
  bool operator!=(const ListIterator& li) const;

 private:
  ListIterator(const list<T>* l, Node* cur);

  void insertNodeBeforeIterator(Node* node);

  Node* current_;
  const list<T>* list_;
};

template <typename T>
list<T>::ListIterator::ListIterator(const ListIterator& li)
    : current_(li.current_), list_(li.list_) {}

template <typename T>
list<T>::ListIterator::ListIterator(const list<T>* l, Node* cur)
    : current_(cur), list_(l) {}

template <typename T>
typename list<T>::reference list<T>::ListIterator::operator*() {
  return current_->data;
}

template <typename T>
typename list<T>::ListIterator& list<T>::ListIterator::operator++() {
  current_ = current_->next;
  return *this;
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator++(int) {
  ListIterator temp(*this);
  ++(*this);
  return temp;
}

template <typename T>
typename list<T>::ListIterator& list<T>::ListIterator::operator--() {
  if (current_ != nullptr) {
    current_ = current_->prev;
  } else {
    current_ = list_->tail_;
  }
  return *this;
}

template <typename T>
typename list<T>::ListIterator list<T>::ListIterator::operator--(int) {
  ListIterator temp(*this);
  --(*this);
  return temp;
}

template <typename T>
bool list<T>::ListIterator::operator==(const ListIterator& li) const {
  return current_ == li.current_ &&
         (current_ != nullptr || list_->tail_ == li.list_->tail_);
}

template <typename T>
bool list<T>::ListIterator::operator!=(const ListIterator& li) const {
  return current_ != li.current_ ||
         (current_ == nullptr && list_->tail_ != li.list_->tail_);
}

template <typename T>
void list<T>::ListIterator::insertNodeBeforeIterator(Node* node) {
  if (current_ != nullptr) {
    node->insertBetweenNodes(current_->prev, current_);
  } else {
    node->insertBetweenNodes(list_->tail_, current_);
  }
}

template <typename T>
class list<T>::ListConstIterator : public list<T>::ListIterator {
  friend class list<T>;

 public:
  ListConstIterator() = default;
  ListConstIterator(const ListIterator& li);
  using list<T>::ListIterator::ListIterator;
  const_reference operator*();

 private:
  using list<T>::ListIterator::operator*;
};

template <typename T>
list<T>::ListConstIterator::ListConstIterator(const list<T>::ListIterator& li)
    : list<T>::ListIterator(li) {}

template <typename T>
typename list<T>::const_reference list<T>::ListConstIterator::operator*() {
  return ListIterator::operator*();
}

template <typename T>
struct list<T>::Node {
  value_type data;
  Node* prev;
  Node* next;

  Node(const value_type& data);
  Node(Node* prev, Node* next, const value_type& data);

  void insertBetweenNodes(Node* prev, Node* next);
};

template <typename T>
list<T>::Node::Node(const value_type& data)
    : data(data), prev(nullptr), next(nullptr) {}

template <typename T>
list<T>::Node::Node(Node* prev, Node* next, const value_type& data)
    : data(data), prev(prev), next(next) {
  insertBetweenNodes(prev, next);
}

template <typename T>
void list<T>::Node::insertBetweenNodes(Node* prev, Node* next) {
  this->prev = prev;
  this->next = next;
  if (prev != nullptr) {
    prev->next = this;
  }
  if (next != nullptr) {
    next->prev = this;
  }
}

template <typename T>
list<T>::list() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
list<T>::list(size_type n) : head_(nullptr), tail_(nullptr), size_(0) {
  for (size_type i = 0; i < n; ++i) {
    push_back(T());
  }
  size_ = n;
}

template <typename T>
list<T>::list(std::initializer_list<value_type> const& items)
    : head_(nullptr), tail_(nullptr), size_(0) {
  for (auto item : items) {
    push_back(item);
  }
}

template <typename T>
list<T>::list(const list& l) : head_(nullptr), tail_(nullptr), size_(0) {
  for (auto iter = l.cbegin(); iter != l.cend(); ++iter) {
    push_back(*iter);
  }
}

template <typename T>
list<T>::list(list&& l) {
  head_ = l.head_;
  tail_ = l.tail_;
  size_ = l.size_;

  l.head_ = nullptr;
  l.tail_ = nullptr;
  l.size_ = 0;
}

template <typename T>
list<T>::~list() {
  deallocate();
}

template <typename T>
list<T>& list<T>::operator=(list&& l) {
  if (this != &l) {
    deallocate();
    head_ = l.head_;
    tail_ = l.tail_;
    size_ = l.size_;

    l.head_ = nullptr;
    l.tail_ = nullptr;
    l.size_ = 0;
  }
  return *this;
}

template <typename T>
typename list<T>::const_reference list<T>::front() const {
  return head_->data;
}

template <typename T>
typename list<T>::const_reference list<T>::back() const {
  return tail_->data;
}

template <typename T>
typename list<T>::iterator list<T>::begin() {
  return ListIterator(this, head_);
}

template <typename T>
typename list<T>::const_iterator list<T>::cbegin() const {
  return ListConstIterator(this, head_);
}

template <typename T>
typename list<T>::iterator list<T>::end() {
  return ListIterator(this, nullptr);
}

template <typename T>
typename list<T>::const_iterator list<T>::cend() const {
  return ListConstIterator(this, nullptr);
}

template <typename T>
bool list<T>::empty() const {
  return size_ == 0;
}

template <typename T>
typename list<T>::size_type list<T>::size() const {
  return size_;
}

template <typename T>
typename list<T>::size_type list<T>::max_size() const {
  return std::numeric_limits<size_type>::max() / sizeof(Node);
}

template <typename T>
void list<T>::clear() {
  deallocate();
}

template <typename T>
typename list<T>::iterator list<T>::insert(iterator pos,
                                           const_reference value) {
  auto return_pos = begin();
  if (size_) {
    Node* inserted = new Node(value);
    pos.insertNodeBeforeIterator(inserted);

    if (pos == begin()) {
      head_ = inserted;
    } else if (pos == end()) {
      tail_ = inserted;
    }

    ++size_;
    return_pos = ListIterator(this, inserted);
  } else {
    push_back(value);
  }
  return return_pos;
}

template <typename T>
void list<T>::erase(iterator pos) {
  eraseNode(pos.current_);
}

template <typename T>
void list<T>::push_back(const_reference value) {
  Node* node = new Node(tail_, nullptr, value);
  if (empty()) {
    head_ = node;
  }
  tail_ = node;
  ++size_;
}

template <typename T>
void list<T>::pop_back() {
  eraseNode(tail_);
}

template <typename T>
void list<T>::push_front(const_reference value) {
  Node* node = new Node(nullptr, head_, value);
  if (empty()) {
    tail_ = node;
  }
  head_ = node;
  ++size_;
}

template <typename T>
void list<T>::pop_front() {
  eraseNode(head_);
}

template <typename T>
void list<T>::swap(list<T>& other) {
  std::swap(*this, other);
}

template <typename T>
void list<T>::merge(list<T>& other) {
  if (this != &other) {
    auto iter = begin();
    while (other.size() > 0) {
      if (iter == end() || *iter > other.front()) {
        auto node = other.detachNode(other.head_);
        iter = insertNode(iter, node);
      } else {
        ++iter;
      }
    }
  }
}

template <typename T>
void list<T>::splice(const_iterator pos, list& other) {
  auto iter = ListIterator(this, pos.current_);
  while (other.size() > 0) {
    auto node = other.detachNode(other.head_);
    insertNode(iter, node);
  }
}

template <typename T>
void list<T>::reverse() {
  Node* current = head_;
  while (current != nullptr) {
    std::swap(current->prev, current->next);
    current = current->prev;
  }
  std::swap(head_, tail_);
}

template <typename T>
void list<T>::unique() {
  if (size_ >= 2) {
    auto iter = begin();
    ++iter;
    for (; iter != end(); ++iter) {
      iterator prev(iter);
      --prev;
      if (*iter == *prev) {
        eraseNode(prev.current_);
      }
    }
  }
}

template <typename T>
void list<T>::sort() {
  if (size_ >= 2) {
    list<T> second_half = splitForSort();
    sort();
    second_half.sort();
    merge(second_half);
  }
}

template <typename T>
template <typename... Args>
typename list<T>::iterator list<T>::insert_many(const_iterator pos,
                                                Args&&... args) {
  auto iter = ListIterator(this, pos.current_);
  if constexpr (sizeof...(args) > 0) {
    (insert(iter, value_type(std::forward<Args>(args))), ...);
    for (size_type i = 0; i < sizeof...(args); ++i) {
      --iter;
    }
  }
  return iter;
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_back(Args&&... args) {
  (push_back(value_type(std::forward<Args>(args))), ...);
}

template <typename T>
template <typename... Args>
void list<T>::insert_many_front(Args&&... args) {
  insert_many(begin(), args...);
}

template <typename T>
void list<T>::deallocate() {
  Node* current = head_;
  Node* next = nullptr;
  while (current != nullptr) {
    next = current->next;
    delete current;
    current = next;
  }
  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
}

template <typename T>
typename list<T>::Node* list<T>::detachNode(iterator pos) {
  return detachNode(pos.current_);
}

template <typename T>
typename list<T>::Node* list<T>::detachNode(Node* node) {
  if (node == head_) head_ = head_->next;
  if (node == tail_) tail_ = tail_->prev;
  if (node->prev != nullptr) node->prev->next = node->next;
  if (node->next != nullptr) node->next->prev = node->prev;
  node->prev = nullptr;
  node->next = nullptr;
  --size_;
  return node;
}

template <typename T>
void list<T>::eraseNode(Node* node) {
  Node* node_to_erase = detachNode(node);
  delete node_to_erase;
}

template <typename T>
typename list<T>::iterator list<T>::insertNode(iterator pos, Node* node) {
  if (size_ == 0) {
    head_ = node;
    tail_ = node;
  } else {
    pos.insertNodeBeforeIterator(node);
    if (pos == begin()) {
      head_ = node;
    } else if (pos == end()) {
      tail_ = node;
    }
  }
  ++size_;
  return ListIterator(this, node);
}

template <typename T>
list<T> list<T>::splitForSort() {
  list<T> second_part;
  Node* fast = head_;
  Node* slow = head_;
  size_type new_size = 1;
  while (fast->next != nullptr && fast->next->next != nullptr) {
    fast = fast->next->next;
    slow = slow->next;
    ++new_size;
  }
  second_part.tail_ = tail_;
  tail_ = slow;
  second_part.head_ = slow->next;
  second_part.head_->prev = nullptr;
  slow->next = nullptr;
  second_part.size_ = size_ - new_size;
  size_ = new_size;
  return second_part;
}

}  // namespace lib

#endif  // LIB_LIST_H_
