#ifndef LIB_QUEUE_H_
#define LIB_QUEUE_H_

#include <cstddef>
#include <initializer_list>
#include <utility>

namespace lib {
template <typename T>
class queue {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_type = size_t;

  queue();
  queue(std::initializer_list<value_type> const& items);
  queue(const queue& q);
  queue(queue&& q);
  ~queue();
  queue& operator=(queue&& q);

  const_reference front() const;
  const_reference back() const;

  bool empty() const;
  size_type size() const;

  void push(const_reference value);
  void pop();
  void swap(queue& other);

  template <typename... Args>
  void insert_many_back(Args&&... args);

 private:
  struct Node {
    T data;
    Node* next;

    Node(const T& value) : data(value), next(nullptr) {}
  };

  Node* head_;
  Node* tail_;
  size_t count_;
};

template <typename T>
queue<T>::queue() : head_(nullptr), tail_(nullptr), count_(0) {}

template <typename T>
queue<T>::queue(std::initializer_list<value_type> const& items) : queue() {
  for (const auto& item : items) {
    push(item);
  }
}

template <typename T>
queue<T>::queue(const queue& q) : queue() {
  Node* current = q.head_;
  while (current) {
    push(current->data);
    current = current->next;
  }
}

template <typename T>
queue<T>::queue(queue&& q) : head_(q.head_), tail_(q.tail_), count_(q.count_) {
  q.head_ = nullptr;
  q.tail_ = nullptr;
  q.count_ = 0;
}

template <typename T>
queue<T>::~queue() {
  while (head_) {
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
  }
}

template <typename T>
queue<T>& queue<T>::operator=(queue&& q) {
  if (this != &q) {
    std::swap(head_, q.head_);
    std::swap(tail_, q.tail_);
    std::swap(count_, q.count_);
  }
  return *this;
}

template <typename T>
void queue<T>::push(const_reference value) {
  Node* newNode = new Node(value);
  if (!head_) {
    head_ = tail_ = newNode;
  } else {
    tail_->next = newNode;
    tail_ = newNode;
  }
  ++count_;
}

template <typename T>
void queue<T>::pop() {
  if (head_) {
    Node* temp = head_;
    head_ = head_->next;
    delete temp;
    --count_;
    if (!head_) {
      tail_ = nullptr;
    }
  }
}

template <typename T>
void queue<T>::swap(queue& other) {
  std::swap(head_, other.head_);
  std::swap(tail_, other.tail_);
  std::swap(count_, other.count_);
}

template <typename T>
typename queue<T>::const_reference queue<T>::front() const {
  return head_->data;
}

template <typename T>
typename queue<T>::const_reference queue<T>::back() const {
  return tail_->data;
}

template <typename T>
bool queue<T>::empty() const {
  return head_ == nullptr;
}

template <typename T>
typename queue<T>::size_type queue<T>::size() const {
  return count_;
}

template <typename T>
template <typename... Args>
void queue<T>::insert_many_back(Args&&... args) {
  (push(value_type(std::forward<Args>(args))), ...);
}

}  // namespace lib

#endif  // LIB_QUEUE_H_
