#ifndef LIB_STACK_H_
#define LIB_STACK_H_

#include <cstddef>
#include <initializer_list>
#include <utility>

namespace lib {
template <typename T>
class stack {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  stack();
  stack(std::initializer_list<value_type> const& items);
  stack(const stack& s);
  stack(stack&& s);
  ~stack();
  stack& operator=(stack&& s);

  const_reference top() const;

  bool empty() const;
  size_type size() const;

  void push(const_reference value);
  void pop();
  void swap(stack& other);

  template <typename... Args>
  void insert_many_front(Args&&... args);

 private:
  struct Node {
    Node* next;
    T data;

    Node(const T& value) : next(nullptr), data(value) {}
  };

  Node* head_;
  size_t count_;
};

template <typename T>
stack<T>::stack() : head_(nullptr), count_(0) {}

template <typename T>
stack<T>::stack(std::initializer_list<value_type> const& items) : stack() {
  for (const auto& item : items) {
    push(item);
  }
}

template <typename T>
stack<T>::stack(const stack& s) : stack() {
  Node* current = s.head_;
  Node* prev = nullptr;
  while (current) {
    Node* newNode = new Node(current->data);
    if (!head_) {
      head_ = newNode;
    } else {
      prev->next = newNode;
    }
    prev = newNode;
    current = current->next;
  }
  count_ = s.count_;
}

template <typename T>
stack<T>::stack(stack&& s) : head_(s.head_), count_(s.count_) {
  s.head_ = nullptr;
  s.count_ = 0;
}

template <typename T>
stack<T>::~stack() {
  while (head_) {
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
  }
}

template <typename T>
stack<T>& stack<T>::operator=(stack&& s) {
  if (this != &s) {
    std::swap(head_, s.head_);
    std::swap(count_, s.count_);
  }
  return *this;
}

template <typename T>
void stack<T>::push(const_reference value) {
  Node* newNode = new Node(value);
  newNode->next = head_;
  head_ = newNode;
  ++count_;
}

template <typename T>
void stack<T>::pop() {
  if (head_) {
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
    --count_;
  }
}

template <typename T>
void stack<T>::swap(stack& other) {
  std::swap(head_, other.head_);
  std::swap(count_, other.count_);
}

template <typename T>
typename stack<T>::const_reference stack<T>::top() const {
  return head_->data;
}

template <typename T>
bool stack<T>::empty() const {
  return head_ == nullptr;
}

template <typename T>
typename stack<T>::size_type stack<T>::size() const {
  return count_;
}

template <typename T>
template <typename... Args>
void stack<T>::insert_many_front(Args&&... args) {
  (push(value_type(std::forward<Args>(args))), ...);
}

}  // namespace lib

#endif  // LIB_STACK_H_
