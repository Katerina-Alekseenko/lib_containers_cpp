#include <gtest/gtest.h>

#include "../lib_containers.h"

TEST(Vector, DefaultConstructor) {
  lib::vector<int> A;
  EXPECT_EQ(A.data(), nullptr);
  EXPECT_EQ(0, A.size());
}

TEST(Vector, ParameterizedConstructor) {
  lib::vector<double> A(5);
  double* data = A.data();

  EXPECT_NE(data, nullptr);
  EXPECT_EQ(5, A.size());

  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_DOUBLE_EQ(0, data[i]);
  }
}

TEST(Vector, InitializerListConstructor) {
  lib::vector<char> A({'a', 'b', 'c'});
  char* data = A.data();

  EXPECT_NE(data, nullptr);
  EXPECT_EQ(3, A.size());

  EXPECT_EQ('a', data[0]);
  EXPECT_EQ('b', data[1]);
  EXPECT_EQ('c', data[2]);
}

TEST(Vector, CopyConstructor) {
  lib::vector<unsigned> B({1, 2, 3, 4});
  lib::vector A(B);

  unsigned* data = A.data();
  unsigned* dataB = B.data();

  EXPECT_NE(data, dataB);
  EXPECT_EQ(A.size(), B.size());

  for (size_t i = 0; i < A.size(); i++) {
    EXPECT_EQ(data[i], dataB[i]);
  }
}

TEST(Vector, MoveConstructor) {
  lib::vector<unsigned> B({1, 2, 3, 4});
  lib::vector A(std::move(B));

  unsigned* data = A.data();
  unsigned* dataB = B.data();

  EXPECT_NE(data, nullptr);
  EXPECT_EQ(dataB, nullptr);

  EXPECT_EQ(4, A.size());

  for (size_t i = 0; i < A.size(); i++) {
    EXPECT_EQ(i + 1, data[i]);
  }
}

TEST(Vector, CopyAssignment) {
  lib::vector<unsigned> B({1, 2, 3, 4});
  lib::vector<unsigned> A(B);

  EXPECT_EQ(A.size(), B.size());
  EXPECT_EQ(A.capacity(), B.capacity());
  EXPECT_EQ(A.front(), B.front());
  EXPECT_EQ(A.back(), B.back());
  EXPECT_EQ(A.at(1), B.at(1));
  EXPECT_EQ(A.empty(), B.empty());

  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(B[i], A[i]);
  }
}

TEST(Vector, MoveAssignment) {
  lib::vector<unsigned> B({1, 2, 3, 4});
  lib::vector<unsigned> A(15);

  A = std::move(B);

  unsigned* data = A.data();
  unsigned* dataB = B.data();

  EXPECT_NE(data, nullptr);
  EXPECT_EQ(dataB, nullptr);

  EXPECT_EQ(4, A.size());

  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(i + 1, data[i]);
  }
}

TEST(Vector, InitializerListAssignment) {
  lib::vector<unsigned> A(10);
  A = {1, 2, 3, 4};
  EXPECT_EQ(4, A.size());
  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(i + 1, A[i]);
  }
}

TEST(Vector, ElementAccessAtNormal) {
  lib::vector<int> A({1, 2, 3});
  int* data = A.data();

  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(data[i], A.at(i));
  }
}

TEST(Vector, ElementAccessAtOutOfRange) {
  EXPECT_ANY_THROW({
    lib::vector<double> A;
    A.at(0);
  });
}

TEST(Vector, ElementAccessAtOutOfRange2) {
  EXPECT_ANY_THROW({
    lib::vector<int> A({1, 2, 3});
    A.at(3);
  });
}

TEST(Vector, ElementAccessOperator) {
  lib::vector<char> A({'a', 'b', 'c', 'd'});
  char* data = A.data();

  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(data[i], A[i]);
  }
}

TEST(Vector, ElementAccessFront) {
  lib::vector<char> A({'a', 'b', 'c', 'd'});
  EXPECT_EQ('a', A.front());

  lib::vector<double> B({2e22});
  EXPECT_DOUBLE_EQ(2e22, B.front());

  lib::vector<void*> C(5);
  EXPECT_EQ(nullptr, C.front());
}

TEST(Vector, ElementAccessBack) {
  lib::vector<char> A({'a', 'b', 'c', 'd'});
  EXPECT_EQ('d', A.back());

  lib::vector<double> B({2e22});
  EXPECT_DOUBLE_EQ(2e22, B.back());

  lib::vector<void*> C(5);
  EXPECT_EQ(nullptr, C.back());
}

TEST(Vector, ElementAccessIter) {
  lib::vector<unsigned> A({0, 2, 4, 6, 8, 10, 12});
  unsigned* data = A.data();
  for (auto iter = A.begin(); iter != A.end(); ++iter) {
    EXPECT_EQ(*(data++), *iter);
  }
  data--;
  for (auto iter = A.end() - 1; iter != A.begin(); --iter) {
    EXPECT_EQ(*(data--), *iter);
  }
  auto iter1 = A.begin();
  auto iter2 = A.end() - 1;
  while (iter1 != iter2) {
    ++iter1;
    --iter2;
  }
  EXPECT_EQ(iter1, iter2);
}

TEST(Vector, IteratorsBegin) {
  lib::vector<int> A = {1, 2, 3};
  auto example = A.begin();
  EXPECT_EQ(A.front(), *example);
}

TEST(Vector, IteratorsBeginEmpty) {
  lib::vector<int> A;
  EXPECT_EQ(A.begin(), nullptr);
}

TEST(Vector, IteratorsEnd) {
  lib::vector<int> A = {1, 2, 3};
  auto example = A.end() - 1;
  EXPECT_EQ(A.back(), *example);
}

TEST(Vector, IteratorsEndEmpty) {
  lib::vector<int> A;
  EXPECT_EQ(A.end(), nullptr);
}

TEST(Vector, CapacityEmpty) {
  lib::vector<int> A;
  EXPECT_TRUE(A.empty());
}

TEST(Vector, CapacityNotEmpty) {
  lib::vector<int> A = {1, 2, 3};
  EXPECT_FALSE(A.empty());
}

TEST(Vector, CapacitySize) {
  lib::vector<int> A = {1, 2, 3};
  EXPECT_EQ(A.size(), 3);
}

TEST(Vector, CapacitySizeZero) {
  lib::vector<int> A;
  EXPECT_EQ(A.size(), 0);
}

TEST(Vector, CapacityReserve) {
  lib::vector<unsigned> A;
  auto cap = A.capacity();

  A.reserve(15);
  cap = 15;
  EXPECT_EQ(cap, A.capacity());

  A.reserve(200);
  cap = 200;
  EXPECT_EQ(cap, A.capacity());

  A.reserve(100);
  EXPECT_EQ(cap, A.capacity());

  EXPECT_EQ(0, A.size());
}

TEST(Vector, Capacity) {
  lib::vector<int> A;
  EXPECT_EQ(A.capacity(), 0);
  lib::vector<int> B(3);
  EXPECT_EQ(B.capacity(), 3);
}

TEST(Vector, CapacityShrinkToFit) {
  lib::vector<int> A = {1, 2, 3};
  A.reserve(10);
  std::vector<int> B = {1, 2, 3};
  B.reserve(10);
  EXPECT_EQ(A.size(), B.size());
  EXPECT_EQ(A.capacity(), B.capacity());
  EXPECT_EQ(A.capacity(), 10);
  A.shrink_to_fit();
  B.shrink_to_fit();
  EXPECT_EQ(A.size(), B.size());
  EXPECT_EQ(A.capacity(), B.capacity());
  EXPECT_EQ(A.capacity(), 3);
  auto i = 0;
  for (auto it = B.begin(); it != B.end(); it++) {
    EXPECT_EQ(A.at(i), *it);
    i++;
  }
}

TEST(Vector, ModifiersClear) {
  lib::vector<int> A;
  A.clear();
  EXPECT_EQ(0, A.size());

  lib::vector<char> B({'a', 'b', 'c'});
  B.clear();
  EXPECT_EQ(0, B.size());
}

TEST(Vector, ModifiersInsert) {
  lib::vector<char> A({'a'});

  auto pos = A.insert(A.begin(), 'b');
  EXPECT_EQ('b', *pos);

  pos = A.begin();
  pos = A.insert(++pos, 'c');
  EXPECT_EQ('c', *pos);

  EXPECT_EQ(3, A.size());
  EXPECT_EQ('b', A[0]);
  EXPECT_EQ('c', A[1]);
  EXPECT_EQ('a', A[2]);

  A.insert(A.begin(), 'd');
  pos = A.begin();
  ++pos;
  ++pos;
  A.insert(pos, 'e');

  EXPECT_EQ(5, A.size());
  EXPECT_EQ('d', A[0]);
  EXPECT_EQ('b', A[1]);
  EXPECT_EQ('e', A[2]);
  EXPECT_EQ('c', A[3]);
  EXPECT_EQ('a', A[4]);
}

TEST(Vector, ModifiersInsertEnd) {
  lib::vector<char> A({'a'});

  auto pos = A.insert(A.end(), 'b');
  EXPECT_EQ('b', *pos);

  pos = A.insert(A.end(), 'c');
  EXPECT_EQ('c', *pos);

  EXPECT_EQ(3, A.size());
  EXPECT_EQ('a', A[0]);
  EXPECT_EQ('b', A[1]);
  EXPECT_EQ('c', A[2]);
}

TEST(Vector, ModifiersInsertEmpty) {
  lib::vector<int> A;

  auto pos = A.insert(A.begin(), 11);
  EXPECT_EQ(11, *pos);

  EXPECT_EQ(1, A.size());
  EXPECT_EQ(11, A[0]);

  lib::vector<double> B;

  auto pos2 = B.insert(B.end(), 2.33);
  EXPECT_DOUBLE_EQ(2.33, *pos2);

  EXPECT_EQ(1, B.size());
  EXPECT_DOUBLE_EQ(2.33, B[0]);
}

TEST(Vector, ModifiersErase) {
  lib::vector<int> A({1, 2, 3, 4, 5});

  auto pos = A.begin();
  ++pos;
  ++pos;

  A.erase(pos);

  EXPECT_EQ(4, A.size());
  EXPECT_EQ(1, A[0]);
  EXPECT_EQ(2, A[1]);
  EXPECT_EQ(4, A[2]);
  EXPECT_EQ(5, A[3]);

  pos = A.end();
  --pos;

  A.erase(pos);

  EXPECT_EQ(3, A.size());
  EXPECT_EQ(1, A[0]);
  EXPECT_EQ(2, A[1]);
  EXPECT_EQ(4, A[2]);

  A.erase(A.begin());

  EXPECT_EQ(2, A.size());
  EXPECT_EQ(2, A[0]);
  EXPECT_EQ(4, A[1]);
}

TEST(Vector, ModifiersPushBack) {
  lib::vector<int> A;

  for (int i = 0; i < 100; ++i) {
    A.push_back(i);
  }

  EXPECT_EQ(100, A.size());

  A.shrink_to_fit();

  for (int i = 100; i < 1000; ++i) {
    A.push_back(i);
  }

  EXPECT_EQ(1000, A.size());

  for (int i = 0; i < 1000; ++i) {
    EXPECT_EQ(i, A[i]);
  }
}

TEST(Vector, ModifiersPopBack) {
  lib::vector<int> A;

  for (int i = 0; i < 2000; ++i) {
    A.push_back(i);
  }

  for (int i = 0; i < 500; ++i) {
    A.pop_back();
  }

  EXPECT_EQ(1500, A.size());

  for (int i = 0; i < 1500; ++i) {
    EXPECT_EQ(i, A.at(i));
  }

  for (int i = 0; i < 1500; ++i) {
    A.pop_back();
  }

  EXPECT_EQ(0, A.size());
}

TEST(Vector, ModifiersSwap) {
  lib::vector<int> A({1, 2, 3, 4});
  lib::vector copied_vec(A);
  auto size = A.size();
  auto capacity = A.capacity();
  auto data = A.data();

  lib::vector<int> other_vec({5, 6, 7, 8, 9, 10});
  lib::vector copied_other_vec(other_vec);
  auto other_size = other_vec.size();
  auto other_capacity = other_vec.capacity();
  auto other_data = other_vec.data();

  A.swap(other_vec);

  EXPECT_EQ(A.size(), other_size);
  EXPECT_EQ(other_vec.size(), size);

  EXPECT_EQ(copied_other_vec.size(), A.size());
  EXPECT_EQ(copied_vec.size(), other_vec.size());

  EXPECT_EQ(A.capacity(), other_capacity);
  EXPECT_EQ(other_vec.capacity(), capacity);

  EXPECT_EQ(A.data(), other_data);
  EXPECT_EQ(other_vec.data(), data);

  for (size_t i = 0; i < copied_vec.size(); ++i) {
    EXPECT_EQ(copied_vec[i], other_vec[i]);
  }

  for (size_t i = 0; i < copied_other_vec.size(); ++i) {
    EXPECT_EQ(copied_other_vec[i], A[i]);
  }
}

TEST(Vector, ModifiersSelfSwap) {
  lib::vector<int> A({1, 2, 3, 4});
  lib::vector copied_vec(A);

  A.swap(A);

  EXPECT_EQ(copied_vec.size(), A.size());
  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(copied_vec[i], A[i]);
  }
}

TEST(Vector, BonusModifiersInsertManyOnce) {
  lib::vector<int> A({1, 2, 3});

  auto pos = A.begin();
  ++pos;
  pos = A.insert_many(pos, 4);

  EXPECT_EQ(*pos, 4);

  EXPECT_EQ(4, A.size());
  EXPECT_EQ(1, A[0]);
  EXPECT_EQ(4, A[1]);
  EXPECT_EQ(2, A[2]);
  EXPECT_EQ(3, A[3]);
}

TEST(Vector, BonusModifiersInsertManyMultiple) {
  lib::vector<char> A({'a', 'b', 'c'});

  auto pos = A.begin();
  ++pos;
  ++pos;
  pos = A.insert_many(pos, 'd', 'e', 'f');

  EXPECT_EQ(*pos, 'd');

  EXPECT_EQ(6, A.size());
  EXPECT_EQ('a', A[0]);
  EXPECT_EQ('b', A[1]);
  EXPECT_EQ('d', A[2]);
  EXPECT_EQ('e', A[3]);
  EXPECT_EQ('f', A[4]);
  EXPECT_EQ('c', A[5]);

  pos = A.insert_many(A.begin(), 'a', 'b', std::move('o'), 'b', 'a');

  EXPECT_EQ(*pos, 'a');

  EXPECT_EQ(11, A.size());
  EXPECT_EQ('a', A[0]);
  EXPECT_EQ('b', A[1]);
  EXPECT_EQ('o', A[2]);
  EXPECT_EQ('b', A[3]);
  EXPECT_EQ('a', A[4]);
  EXPECT_EQ('a', A[5]);
  EXPECT_EQ('b', A[6]);
  EXPECT_EQ('d', A[7]);
  EXPECT_EQ('e', A[8]);
  EXPECT_EQ('f', A[9]);
  EXPECT_EQ('c', A[10]);
}

TEST(Vector, BonusModifiersInsertManyNothing) {
  lib::vector<double> A({1.1, 2.2, 3.3});
  auto pos = A.insert_many(A.begin());

  EXPECT_DOUBLE_EQ(*pos, 1.1);

  EXPECT_EQ(3, A.size());
  EXPECT_DOUBLE_EQ(1.1, A[0]);
  EXPECT_DOUBLE_EQ(2.2, A[1]);
  EXPECT_DOUBLE_EQ(3.3, A[2]);
}

TEST(Vector, BonusModifiersInsertManyToEmpty) {
  lib::vector<int> A;
  auto pos = A.insert_many(A.begin(), 1, 2, 3);

  EXPECT_EQ(*pos, 1);

  EXPECT_EQ(3, A.size());
  EXPECT_EQ(1, A[0]);
  EXPECT_EQ(2, A[1]);
  EXPECT_EQ(3, A[2]);
}

TEST(Vector, BonusModifiersInsertManyToEnd) {
  lib::vector<char> A({'a', 'b', 'o'});
  auto pos = A.insert_many(A.end(), 'b', 'u', 's');
  EXPECT_EQ(*pos, 'b');

  EXPECT_EQ(6, A.size());
  EXPECT_EQ('a', A[0]);
  EXPECT_EQ('b', A[1]);
  EXPECT_EQ('o', A[2]);
  EXPECT_EQ('b', A[3]);
  EXPECT_EQ('u', A[4]);
  EXPECT_EQ('s', A[5]);
}

TEST(Vector, BonusModifiersInsertManyBackOnce) {
  lib::vector<int> A({0, 2, 4, 6});
  A.insert_many_back(8);

  EXPECT_EQ(5, A.size());
  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(2 * i, A[i]);
  }
}

TEST(Vector, BonusModifiersInsertManyBackMultiple) {
  lib::vector<int> A({0, 1, 4, 9});
  A.insert_many_back(std::move(16), std::move(25), 36);

  EXPECT_EQ(7, A.size());
  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(i * i, A[i]);
  }
}

TEST(Vector, BonusModifiersInsertManyBackNothing) {
  lib::vector<char> A({'c', 'o', 'o', 'l'});
  A.insert_many_back();

  EXPECT_EQ(4, A.size());
  EXPECT_EQ('c', A[0]);
  EXPECT_EQ('o', A[1]);
  EXPECT_EQ('o', A[2]);
  EXPECT_EQ('l', A[3]);
}

TEST(Vector, BonusModifiersInsertManyBackEmpty) {
  lib::vector<char> A;
  A.insert_many_back('c', 'o', 'o', 'l');

  EXPECT_EQ(4, A.size());
  EXPECT_EQ('c', A[0]);
  EXPECT_EQ('o', A[1]);
  EXPECT_EQ('o', A[2]);
  EXPECT_EQ('l', A[3]);
}
