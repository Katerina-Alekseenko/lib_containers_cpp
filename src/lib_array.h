#ifndef LIB_ARRAY_H
#define LIB_ARRAY_H

#include <iostream>
#include <limits>

namespace lib {
template <typename T, std::size_t my_size>
class array {
 public:
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using iterator = T *;
  using const_iterator = const T *;
  using size_type = size_t;

  array();
  array(std::initializer_list<value_type> const &items);
  array(const array &a);
  array(array &&a) noexcept;
  ~array() = default;

  iterator begin();
  iterator end();
  const_iterator cbegin(void) const;
  const_iterator cend(void) const;
  iterator data();
  const_iterator data() const;

  reference at(size_type pos);
  const_reference at(size_type pos) const;
  reference front();
  reference back();
  const_reference front() const;
  const_reference back() const;

  bool empty();
  size_type size() const;
  size_type max_size();

  void swap(array &a);
  void fill(const_reference value);

  array &operator=(const array &a);
  array &operator=(array &&a) noexcept;
  reference operator[](size_type pos);
  const_reference operator[](size_type pos) const;

 private:
  value_type elems_[my_size] = {};
  size_type size_ = my_size;
};

template <typename T, std::size_t my_size>
array<T, my_size>::array() : size_(my_size) {}

template <typename T, std::size_t my_size>
array<T, my_size>::array(const std::initializer_list<value_type> &items) {
  std::copy(items.begin(), items.end(), elems_);
}

template <typename T, std::size_t my_size>
array<T, my_size>::array(const array &a) {
  std::copy(a.elems_, a.elems_ + my_size, elems_);
}

template <typename T, std::size_t my_size>
array<T, my_size>::array(array &&a) noexcept {
  std::move(a.elems_, a.elems_ + my_size, elems_);
  a.size_ = 0;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::iterator array<T, my_size>::begin() {
  return elems_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::iterator array<T, my_size>::end() {
  return elems_ + size_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_iterator array<T, my_size>::cbegin() const {
  return elems_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_iterator array<T, my_size>::cend() const {
  return elems_ + size_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::reference array<T, my_size>::at(size_type pos) {
  if (pos >= my_size || static_cast<std::make_signed_t<size_type>>(pos) < 0) {
    throw std::out_of_range("Index out of range");
  }
  return elems_[pos];
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_reference array<T, my_size>::at(
    size_type pos) const {
  if (pos >= my_size || static_cast<std::make_signed_t<size_type>>(pos) < 0) {
    throw std::out_of_range("Index out of range");
  }
  return elems_[pos];
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::reference array<T, my_size>::front() {
  return elems_[0];
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_reference array<T, my_size>::front() const {
  return elems_[0];
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::reference array<T, my_size>::back() {
  return elems_[my_size - 1];
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_reference array<T, my_size>::back() const {
  return elems_[my_size - 1];
}

template <typename T, std::size_t my_size>
bool array<T, my_size>::empty() {
  return !size_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::size_type array<T, my_size>::size() const {
  return size_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::size_type array<T, my_size>::max_size() {
  return size();
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::iterator array<T, my_size>::data() {
  return elems_;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_iterator array<T, my_size>::data() const {
  return elems_;
}

template <typename T, std::size_t my_size>
void array<T, my_size>::swap(array<T, my_size> &a) {
  std::swap_ranges(elems_, elems_ + my_size, a.elems_);
}

template <typename T, std::size_t my_size>
void array<T, my_size>::fill(const_reference value) {
  std::fill(elems_, elems_ + my_size, value);
}

template <typename T, std::size_t my_size>
array<T, my_size> &array<T, my_size>::operator=(const array<T, my_size> &a) {
  if (this != &a) {
    std::copy(a.elems_, a.elems_ + my_size, elems_);
  }
  return *this;
}

template <typename T, std::size_t my_size>
array<T, my_size> &array<T, my_size>::operator=(
    array<T, my_size> &&a) noexcept {
  if (this != &a) {
    for (size_t i = 0; i < my_size; ++i) {
      elems_[i] = std::move(a.elems_[i]);
    }
  }

  return *this;
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::reference array<T, my_size>::operator[](
    size_type pos) {
  if (pos >= my_size || static_cast<std::make_signed_t<size_type>>(pos) < 0) {
    throw std::out_of_range("OutOfRangeError: Index out of array range");
  }

  return elems_[pos];
}

template <typename T, std::size_t my_size>
typename array<T, my_size>::const_reference array<T, my_size>::operator[](
    size_type pos) const {
  if (pos >= my_size) {
    throw std::out_of_range("OutOfRangeError: Index out of array range");
  }
  return elems_[pos];
}

}  // namespace lib

#endif  // lib_ARRAY_H
