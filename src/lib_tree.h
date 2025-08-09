#ifndef SRC_LIB_TREE_H_
#define SRC_LIB_TREE_H_

#include "lib_vector.h"

namespace lib {
template <typename Key>
class RBTree {
  class RBNode;
  class RBIterator;
  class RBConstIterator;
  using reference = Key&;
  using const_reference = const Key&;
  using size_type = std::size_t;
  using comparator = std::less<Key>;
  using NodePtr = RBNode*;

  enum NodeColor { BLACK, RED };

 public:
  using iterator = RBIterator;
  using const_iterator = RBConstIterator;
  using value_type = Key;

  RBTree() : root_(new RBNode), size_(0) {}
  RBTree(const RBTree& other) : RBTree() { *this = other; }
  RBTree(RBTree&& other) : RBTree() { *this = std::move(other); }

  ~RBTree() {
    clear();
    eraseNode_(root_);
  }

  RBTree& operator=(const RBTree& other) {
    if (this != &other) {
      if (other.size_ == 0) {
        clear();
      } else {
        if (root_->parent_) clear();
        NodePtr root = copyRBNode_(other.root_->parent_, nullptr);
        root_->parent_ = root;
        root_->parent_->parent_ = root;
        root_->left_ = other.root_->left_;
        root_->right_ = other.root_->right_;
        size_ = other.size_;
      }
    }
    return *this;
  }

  RBTree& operator=(RBTree&& other) {
    if (this != &other) {
      clear();
      std::swap(this->root_, other.root_);
      std::swap(this->size_, other.size_);
    } else {
      other.clear();
      eraseNode_(other.root_);
    }
    return *this;
  }

  iterator begin() noexcept { return iterator(root_->left_); }
  const_iterator begin() const noexcept { return const_iterator(root_->left_); }
  iterator end() noexcept { return iterator(root_); }
  const_iterator end() const noexcept { return const_iterator(root_); }
  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }

  size_type max_size() const noexcept {
    return (std::numeric_limits<size_type>::max() / sizeof(RBNode));
  }

  void clear() {
    eraseTree_(root_->parent_);
    root_->parent_ = nullptr;
    root_->left_ = nullptr;
    root_->right_ = nullptr;
    size_ = 0;
  }

  std::pair<iterator, bool> insertUnique(const value_type& value) {
    NodePtr new_node = new RBNode(value);
    std::pair<iterator, bool> res = insertNode_(new_node, true);
    if (!res.second) eraseNode_(new_node);
    return res;
  }

  iterator insertDuplicate(const value_type& value) {
    NodePtr new_node = new RBNode(value);
    return insertNode_(new_node, false).first;
  }

  void erase(iterator pos) { eraseNode_(pos); };

  void swap(RBTree& other) {
    using std::swap;
    swap(root_, other.root_);
    swap(size_, other.size_);
  }

  iterator find(const_reference key) noexcept {
    iterator res = iterator(findNode_(key));
    if (res == end() || comparator{}(*res, key)) return end();
    return res;
  }

  const_iterator find(const_reference key) const noexcept {
    const_iterator res = lower_bound(key);
    if (res == end() || comparator{}(*res, key)) return end();
    return res;
  }

  bool contains(const_reference key) const noexcept {
    NodePtr node = findNode_(key);
    return (node != root_);
  }

  iterator upper_bound(const_reference value) noexcept {
    iterator result = end();
    NodePtr begin = root_->parent_;
    while (begin != nullptr) {
      if (comparator{}(value, begin->data_)) {
        result = iterator(begin);
        begin = begin->left_;
      } else
        begin = begin->right_;
    }
    return result;
  }

  const_iterator upper_bound(const_reference value) const noexcept {
    const_iterator result = end();
    NodePtr begin = root_->parent_;
    while (begin != nullptr) {
      if (comparator{}(value, begin->data_)) {
        result = const_iterator(begin);
        begin = begin->left_;
      } else
        begin = begin->right_;
    }
    return result;
  }

  iterator lower_bound(const_reference value) noexcept {
    iterator result = end();
    NodePtr begin = root_->parent_;
    while (begin != nullptr) {
      if (comparator{}(begin->data_, value)) {
        begin = begin->right_;
      } else {
        result = iterator(begin);
        begin = begin->left_;
      }
    }
    return result;
  }

  const_iterator lower_bound(const_reference value) const noexcept {
    const_iterator result = end();
    NodePtr begin = root_->parent_;
    while (begin != nullptr) {
      if (comparator{}(begin->data_, value)) {
        begin = begin->right_;
      } else {
        result = const_iterator(begin);
        begin = begin->left_;
      }
    }
    return result;
  }

  size_type count(const_reference key) const noexcept {
    const_iterator it = begin();
    size_type c = 0;
    for (size_type i = 0; i < size(); i++) {
      if (*it == key) c++;
      it++;
    }
    return c;
  }

  std::pair<iterator, iterator> equal_range(const_reference key) noexcept {
    iterator start = lower_bound(key), end = upper_bound(key);
    return {start, end};
  }

  std::pair<const_iterator, const_iterator> equal_range(
      const_reference key) const noexcept {
    const_iterator start = lower_bound(key), end = upper_bound(key);
    return {start, end};
  }

  void mergeDuplicates(RBTree& other) noexcept {
    if (this != &other) {
      iterator it = other.begin();
      while (other.size_ > 0) {
        NodePtr node = it.node_;
        it++;
        if (node->parent_->left_ == node) node->parent_->left_ = nullptr;
        if (node->parent_->right_ == node) node->parent_->right_ = nullptr;
        if (node->right_) node->right_->parent_ = node->parent_;
        if (node->left_) node->left_->parent_ = node->parent_;
        node->left_ = nullptr;
        node->right_ = nullptr;
        node->parent_ = nullptr;
        node->color_ = RED;
        insertNode_(node, false);
        other.size_--;
      }
    }
    other.root_->parent_ = nullptr;
    other.size_ = 0;
  }

  void mergeUnique(RBTree& other) noexcept {
    if (this != &other) {
      iterator it = other.begin();
      while (it != other.end()) {
        if (find(it.node_->data_) == end()) {
          NodePtr node = it.node_;
          it++;
          node = other.extractNode_(node);
          insertNode_(node, true);
        } else
          it++;
      }
    }
  }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insertMany(Args&&... args) {
    vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));
    auto insert_node = [this](auto&& arg) {
      NodePtr new_node = new RBNode(std::forward<decltype(arg)>(arg));
      auto res = insertNode_(new_node, true);
      if (!res.second) {
        delete new_node;
      }
      return res;
    };

    (result.push_back(insert_node(std::forward<Args>(args))), ...);
    return result;
  }

  template <typename... Args>
  vector<std::pair<iterator, bool>> insertManyDuplicate(Args&&... args) {
    vector<std::pair<iterator, bool>> result;
    result.reserve(sizeof...(args));
    ((result.push_back(
         {insertNode_(new RBNode(std::forward<Args>(args)), false)})),
     ...);
    return result;
  }

 private:
  NodePtr root_;
  size_type size_;

  std::pair<iterator, bool> insertNode_(NodePtr new_node, bool unique) {
    NodePtr node = root_->parent_;
    NodePtr parent = nullptr;
    while (node != nullptr) {
      parent = node;
      if (comparator{}(new_node->data_, node->data_))
        node = node->left_;
      else if (comparator{}(node->data_, new_node->data_))
        node = node->right_;
      else if (unique == false)
        node = node->right_;
      else
        return {iterator(node), false};
    }
    size_++;
    if (parent == nullptr) {
      new_node->parent_ = root_;
      root_->parent_ = new_node;
      new_node->color_ = BLACK;
    } else {
      new_node->parent_ = parent;
      comparator{}(new_node->data_, parent->data_) ? parent->left_ = new_node
                                                   : parent->right_ = new_node;
    }
    if (!root_->right_ || root_->right_->right_) {
      root_->right_ = new_node;
    }
    if (!root_->left_ || root_->left_->left_) root_->left_ = new_node;
    balanceAfterInsert_(new_node);
    return {iterator(new_node), true};
  }

  void eraseNode_(NodePtr node) {
    if (node != nullptr) {
      node->left_ = nullptr;
      node->right_ = nullptr;
      node->parent_ = nullptr;
      delete node;
      node = nullptr;
    }
  }

  void leftRotate_(NodePtr node) noexcept {
    NodePtr help_node = node->right_;
    node->right_ = help_node->left_;
    if (help_node->left_ != nullptr) {
      help_node->left_->parent_ = node;
    }
    help_node->parent_ = node->parent_;
    if (node->parent_ == root_) {
      root_->parent_ = help_node;
    } else if (node == node->parent_->left_) {
      node->parent_->left_ = help_node;
    } else {
      node->parent_->right_ = help_node;
    }
    help_node->left_ = node;
    node->parent_ = help_node;
  }

  void rightRotate_(NodePtr node) noexcept {
    NodePtr help_node = node->left_;
    node->left_ = help_node->right_;
    if (help_node->right_ != nullptr) {
      help_node->right_->parent_ = node;
    }
    help_node->parent_ = node->parent_;
    if (root_->parent_ == node) {
      root_->parent_ = help_node;
    } else if (node == node->parent_->right_) {
      node->parent_->right_ = help_node;
    } else if (node == node->parent_->left_) {
      node->parent_->left_ = help_node;
    }
    help_node->right_ = node;
    node->parent_ = help_node;
  }

  void balanceAfterInsert_(NodePtr node) noexcept {
    NodePtr u;
    while (node->parent_->color_ == RED && node != root_->parent_) {
      if (node->parent_ == node->parent_->parent_->right_) {
        u = node->parent_->parent_->left_;
        if (u != nullptr && u->color_ == RED) {
          u->color_ = BLACK;
          node->parent_->color_ = BLACK;
          node->parent_->parent_->color_ = RED;
          node = node->parent_->parent_;
        } else {
          if (node == node->parent_->left_) {
            node = node->parent_;
            rightRotate_(node);
          }
          node->parent_->color_ = BLACK;
          node->parent_->parent_->color_ = RED;
          leftRotate_(node->parent_->parent_);
        }
      } else {
        u = node->parent_->parent_->right_;
        if (u != nullptr && u->color_ == RED) {
          u->color_ = BLACK;
          node->parent_->color_ = BLACK;
          node->parent_->parent_->color_ = RED;
          node = node->parent_->parent_;
        } else {
          if (node == node->parent_->right_) {
            node = node->parent_;
            leftRotate_(node);
          }
          node->parent_->color_ = BLACK;
          node->parent_->parent_->color_ = RED;
          rightRotate_(node->parent_->parent_);
        }
      }
    }
    root_->parent_->color_ = BLACK;
  }

  NodePtr searchRight_(NodePtr node) noexcept {
    while (node->right_) {
      node = node->right_;
    }
    return node;
  }

  NodePtr searchLeft_(NodePtr node) noexcept {
    while (node->left_) {
      node = node->left_;
    }
    return node;
  }

  void swapNodes_(NodePtr one, NodePtr two) noexcept {
    two == two->parent_->left_ ? two->parent_->left_ = one
                               : two->parent_->right_ = one;
    if (one == root_->parent_)
      root_->parent_ = two;
    else
      one == one->parent_->left_ ? one->parent_->left_ = two
                                 : one->parent_->right_ = two;
    std::swap(one->left_, two->left_);
    std::swap(one->right_, two->right_);
    std::swap(one->parent_, two->parent_);
    std::swap(one->color_, two->color_);
    if (one->left_) one->left_->parent_ = one;
    if (one->right_) one->right_->parent_ = one;
    if (two->left_) two->left_->parent_ = two;
    if (two->right_) two->right_->parent_ = two;
  }

  NodePtr findNode_(const_reference key) const noexcept {
    NodePtr ptr = root_->parent_;
    while (ptr) {
      if (ptr->data_ == key) return ptr;
      if (comparator{}(ptr->data_, key))
        ptr = ptr->right_;
      else
        ptr = ptr->left_;
    }
    return root_;
  }

  NodePtr findNode_(iterator pos) const noexcept {
    NodePtr res = findNode_(*pos);
    return res;
  }

  void eraseNode_(iterator pos) {
    if (pos != end()) {
      NodePtr node = pos.node_;
      if (node->left_ && node->right_) {
        NodePtr swap_node = searchRight_(node->left_);
        swapNodes_(node, swap_node);
      }
      if (!node->left_ && node->right_ != nullptr) {
        swapNodes_(node, node->right_);
      }
      if (!node->right_ && node->left_ != nullptr) {
        swapNodes_(node, node->left_);
      }
      if (node->color_ == BLACK && (!node->left_ && !node->right_)) {
        balanceAfterDelete_(node);
      }
      if (root_->parent_ == node) {
        root_->parent_ = nullptr;
        root_->right_ = nullptr;
        root_->left_ = nullptr;
      } else {
        node->parent_->left_ == node ? node->parent_->left_ = nullptr
                                     : node->parent_->right_ = nullptr;
        if (root_->left_ == node) root_->left_ = searchLeft_(root_->parent_);
        if (root_->right_ == node) root_->right_ = searchRight_(root_->parent_);
      }
      eraseNode_(node);
      size_--;
    }
  }

  NodePtr extractNode_(NodePtr node) {
    if (node == root_) {
      return nullptr;
    } else {
      if (node->right_ && node->left_) {
        NodePtr swap = searchRight_(node->left_);
        swapNodes_(node, swap);
      }
      if (node->right_ && !node->left_) swapNodes_(node, node->right_);
      if (node->left_ && !node->right_) swapNodes_(node, node->left_);
      if (node->color_ == BLACK && (!node->right_ && !node->left_))
        balanceAfterDelete_(node);
      if (root_->left_ == node) root_->left_ = node->successor();
      if (root_->right_ == node) root_->right_ = node->predecessor();
      if (root_->parent_ == node)
        root_->parent_ = nullptr;
      else
        node->parent_->left_ == node ? node->parent_->left_ = nullptr
                                     : node->parent_->right_ = nullptr;
      size_--;
      node->left_ = nullptr;
      node->right_ = nullptr;
      node->parent_ = nullptr;
      node->color_ = RED;
      return node;
    }
  }

  void balanceAfterDelete_(NodePtr node) {
    while (node != root_->parent_ && node->color_ == BLACK) {
      NodePtr sibling = (node == node->parent_->left_) ? node->parent_->right_
                                                       : node->parent_->left_;

      if (sibling->color_ == RED) {
        sibling->color_ = BLACK;
        node->parent_->color_ = RED;
        if (node == node->parent_->left_) {
          leftRotate_(node->parent_);
          sibling = node->parent_->right_;
        } else {
          rightRotate_(node->parent_);
          sibling = node->parent_->left_;
        }
      }

      if ((!sibling->left_ || sibling->left_->color_ == BLACK) &&
          (!sibling->right_ || sibling->right_->color_ == BLACK)) {
        sibling->color_ = RED;
        node = node->parent_;
      } else {
        if (node == node->parent_->left_) {
          if (!sibling->right_ || sibling->right_->color_ == BLACK) {
            sibling->left_->color_ = BLACK;
            sibling->color_ = RED;
            rightRotate_(sibling);
            sibling = node->parent_->right_;
          }
          sibling->color_ = node->parent_->color_;
          node->parent_->color_ = BLACK;
          if (sibling->right_) sibling->right_->color_ = BLACK;
          leftRotate_(node->parent_);
        } else {
          if (!sibling->left_ || sibling->left_->color_ == BLACK) {
            sibling->right_->color_ = BLACK;
            sibling->color_ = RED;
            leftRotate_(sibling);
            sibling = node->parent_->left_;
          }
          sibling->color_ = node->parent_->color_;
          node->parent_->color_ = BLACK;
          if (sibling->left_) sibling->left_->color_ = BLACK;
          rightRotate_(node->parent_);
        }
        node = root_->parent_;
      }
    }
    node->color_ = BLACK;
  }

  void eraseTree_(NodePtr node) {
    if (node == nullptr) return;
    eraseTree_(node->left_);
    eraseTree_(node->right_);
    eraseNode_(node);
  }

  NodePtr copyRBNode_(NodePtr source_node, NodePtr parent) {
    if (!source_node) return nullptr;
    NodePtr new_node = new RBNode(source_node);
    new_node->parent_ = parent;
    if (source_node->left_)
      new_node->left_ = copyRBNode_(source_node->left_, new_node);
    if (source_node->right_)
      new_node->right_ = copyRBNode_(source_node->right_, new_node);
    return new_node;
  }

  class RBNode {
   public:
    RBNode()
        : data_(Key{}),
          color_(RED),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr) {}

    RBNode(const Key& value)
        : data_(value),
          color_(RED),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr) {}

    RBNode(const Key&& value)
        : data_(std::move(value)),
          color_(RED),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr) {}

    RBNode(const RBNode* node)
        : data_(node->data_),
          color_(node->color_),
          parent_(nullptr),
          left_(nullptr),
          right_(nullptr) {}

    Key data_;
    NodeColor color_;
    NodePtr parent_;
    NodePtr left_;
    NodePtr right_;

    NodePtr successor() noexcept {
      NodePtr node = this;
      if (node->color_ == RED &&
          (node->parent_ == nullptr || node->parent_->parent_ == node)) {
        return node->right_;
      } else if (node->right_ != nullptr) {
        node = node->right_;
        while (node->left_ != nullptr) node = node->left_;
      } else {
        NodePtr parent = node->parent_;
        while (node == parent->right_) {
          node = parent;
          parent = parent->parent_;
        }
        if (node->right_ != parent) node = parent;
      }
      return node;
    }

    NodePtr predecessor() noexcept {
      NodePtr node = this;
      if (node->color_ == RED &&
          (node->parent_ == nullptr || node->parent_->parent_ == node))
        return node->right_;
      else if (node->left_ != nullptr) {
        node = node->left_;
        while (node->right_ != nullptr) node = node->right_;
      } else {
        NodePtr parent = node->parent_;
        while (node == parent->left_) {
          node = parent;
          parent = parent->parent_;
        }
        if (node->left_ != parent) node = parent;
      }
      return node;
    }
  };

  class RBIterator {
    friend RBTree;

   public:
    RBIterator() : node_(nullptr) {}
    RBIterator(NodePtr node) : node_(node) {}
    reference operator*() noexcept { return node_->data_; }
    bool operator==(const iterator& other) const noexcept {
      return node_ == other.node_;
    }
    bool operator!=(const iterator& other) const noexcept {
      return node_ != other.node_;
    }

    iterator& operator++() noexcept {
      node_ = node_->successor();
      return *this;
    }

    iterator operator++(int) noexcept {
      iterator temp(node_);
      ++(*this);
      return temp;
    }

    iterator& operator--() noexcept {
      node_ = node_->predecessor();
      return *this;
    }

    iterator operator--(int) noexcept {
      iterator temp(node_);
      --(*this);
      return temp;
    }

   private:
    NodePtr node_;
  };

  class RBConstIterator {
    friend RBTree;

   public:
    RBConstIterator() : node_(nullptr) {}
    RBConstIterator(const iterator& other) { node_ = other.node_; }
    const_reference operator*() const noexcept { return node_->data_; }

    const_iterator operator++() noexcept {
      node_ = node_->successor();
      return *this;
    }

    const_iterator operator++(int) noexcept {
      const_iterator temp(*this);
      ++(*this);
      return temp;
    }

    const_iterator operator--() noexcept {
      node_ = node_->predecessor();
      return *this;
    }

    const_iterator operator--(int) noexcept {
      const_iterator temp(*this);
      --(*this);
      return temp;
    }

   private:
    NodePtr node_;
    friend bool operator==(const const_iterator& it1,
                           const const_iterator& it2) noexcept {
      return it1.node_ == it2.node_;
    }

    friend bool operator!=(const const_iterator& it1,
                           const const_iterator& it2) noexcept {
      return it1.node_ != it2.node_;
    }
  };
};
}  // namespace lib

#endif  // SRC_LIB_TREE_H_
