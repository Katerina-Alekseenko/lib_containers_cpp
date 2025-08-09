#ifndef LIB_VECTOR_H_
#define LIB_VECTOR_H_

#include <cstddef>
#include <initializer_list>
#include <limits>

namespace lib {
template <typename T>
class vector {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;
  using difference_type = std::ptrdiff_t;

  vector();
  explicit vector(size_type n);
  vector(std::initializer_list<value_type> const &items);
  vector(const vector &v);
  vector(vector &&v);
  ~vector();
  vector &operator=(vector &&v);

  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front();
  const_reference back();
  T *data();

  iterator begin();
  iterator end();

  bool empty();
  size_type size();
  size_type max_size();
  void reserve(size_type size);
  size_type capacity();
  void shrink_to_fit();

  void clear();
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value);
  void pop_back();
  void swap(vector &other);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

  template <typename... Args>
  void insert_many_back(Args &&...args);

 private:
  T *p_;
  size_t size_;
  size_t capacity_;

  void resizeIfNeeded(size_type incoming_amount = 1);
  void adjustCapacity(size_type capacity);
  void copyData(value_type *arr, size_type size);
  void pasteData(const value_type *arr, size_type size);
  void deallocate();
};

template <typename T>
vector<T>::vector() : p_(nullptr), size_(0), capacity_(0) {}

template <typename T>
vector<T>::vector(size_type n)
    : p_(new value_type[n]()), size_(n), capacity_(n) {}

template <typename T>
vector<T>::vector(std::initializer_list<value_type> const &items) {
  size_ = items.size();
  capacity_ = size_;
  p_ = new value_type[size_];
  pasteData(items.begin(), items.size());
}

template <typename T>
vector<T>::vector(const vector &v) {
  size_ = v.size_;
  capacity_ = v.capacity_;
  p_ = new value_type[v.capacity_]();
  pasteData(v.p_, v.size_);
}

template <typename T>
vector<T>::vector(vector &&v) {
  size_ = v.size_;
  capacity_ = v.capacity_;
  p_ = v.p_;
  v.size_ = 0;
  v.capacity_ = 0;
  v.p_ = nullptr;
}

template <typename T>
vector<T>::~vector() {
  deallocate();
}

template <typename T>
vector<T> &vector<T>::operator=(vector<T> &&v) {
  if (this == &v) {
    return *this;
  }
  deallocate();
  size_ = v.size_;
  capacity_ = v.capacity_;
  p_ = v.p_;
  v.size_ = 0;
  v.capacity_ = 0;
  v.p_ = nullptr;
  return *this;
}

template <typename T>
typename vector<T>::reference vector<T>::at(size_type pos) {
  if (pos >= size_) {
    throw std::out_of_range("Error: Index out of range");
  }
  return p_[pos];
}

template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type pos) {
  return p_[pos];
}

template <typename T>
typename vector<T>::const_reference vector<T>::front() {
  return p_[0];
}

template <typename T>
typename vector<T>::const_reference vector<T>::back() {
  return p_[size_ - 1];
}

template <typename T>
T *vector<T>::data() {
  return p_;
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() {
  return p_;
}

template <typename T>
typename vector<T>::iterator vector<T>::end() {
  return p_ + size_;
}

template <typename T>
bool vector<T>::empty() {
  return size_ == 0;
}

template <typename T>
typename vector<T>::size_type vector<T>::size() {
  return size_;
}

template <typename T>
typename vector<T>::size_type vector<T>::max_size() {
  return std::numeric_limits<size_type>::max() / sizeof(value_type);
}

template <typename T>
void vector<T>::reserve(size_type size) {
  if (size > capacity_) {
    auto resize = new value_type[size]();
    copyData(resize, size);
    deallocate();
    p_ = resize;
    capacity_ = size;
  }
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() {
  return capacity_;
}

template <typename T>
void vector<T>::shrink_to_fit() {
  adjustCapacity(size_);
}

template <typename T>
void vector<T>::clear() {
  size_ = 0;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(iterator pos,
                                               const_reference value) {
  if (pos == end()) {
    push_back(value);
    pos = end();
    return --pos;
  }
  difference_type offset = pos - begin();
  resizeIfNeeded();
  auto insert_pos = begin() + offset;
  pos = insert_pos;
  value_type displaced_item = *pos;
  *pos = value;
  pos++;
  for (; pos <= end(); ++pos) {
    std::swap(*pos, displaced_item);
  }
  ++size_;
  return insert_pos;
}

template <typename T>
template <typename... Args>
typename vector<T>::iterator vector<T>::insert_many(const_iterator pos,
                                                    Args &&...args) {
  difference_type offset = pos - begin();
  resizeIfNeeded(sizeof...(args));
  auto return_pos = begin() + offset;
  (insert(return_pos++, value_type(std::forward<Args>(args))), ...);
  return_pos = begin() + offset;
  return return_pos;
}

template <typename T>
template <typename... Args>
void vector<T>::insert_many_back(Args &&...args) {
  resizeIfNeeded(sizeof...(args));
  (push_back(value_type(std::forward<Args>(args))), ...);
}

template <typename T>
void vector<T>::erase(iterator pos) {
  auto next = pos;
  next++;
  for (; next < end(); ++next, ++pos) {
    std::swap(*next, *pos);
  }
  --size_;
}

template <typename T>
void vector<T>::push_back(const_reference value) {
  resizeIfNeeded();
  p_[size_++] = value;
}

template <typename T>
void vector<T>::pop_back() {
  --size_;
}

template <typename T>
void vector<T>::swap(vector &other) {
  std::swap(*this, other);
}

template <typename T>
void vector<T>::resizeIfNeeded(size_type incoming_amount) {
  if (capacity_ <= size_ + incoming_amount) {
    if (capacity_ == 0) capacity_ = 1;
    while (capacity_ < (size_ + incoming_amount)) capacity_ = 2 * capacity_;
    auto resize = new value_type[capacity_]();
    copyData(resize, capacity_);
    deallocate();
    p_ = resize;
  }
}

template <typename T>
void vector<T>::adjustCapacity(size_type capacity) {
  auto adjusted = new value_type[capacity]();
  copyData(adjusted, capacity);
  deallocate();
  capacity_ = capacity;
  p_ = adjusted;
}

template <typename T>
void vector<T>::copyData(value_type *arr, size_type size) {
  for (size_type i = 0; i < size && i < size_; ++i) {
    arr[i] = p_[i];
  }
}

template <typename T>
void vector<T>::pasteData(const value_type *arr, size_type size) {
  clear();
  for (size_type i = 0; i < size; ++i) {
    p_[i] = arr[i];
  }
  size_ = size;
}

template <typename T>
void vector<T>::deallocate() {
  delete[] p_;
  p_ = nullptr;
}
}  // namespace lib

#endif  // LIB_VECTOR_H_
