#include <gtest/gtest.h>

#include "../lib_containersplus.h"

TEST(Array, DefaultConstructor) {
  lib::array<int, 0> A;
  EXPECT_EQ(0, A.size());
}

TEST(Array, ParameterizedConstructor) {
  lib::array<double, 24> A;
  EXPECT_EQ(24, A.size());
  for (auto iter = A.begin(); iter != A.end(); ++iter) {
    EXPECT_DOUBLE_EQ(0, *iter);
  }
}

TEST(Array, InitializerListConstructor) {
  lib::array<unsigned, 5> A({1, 2, 3, 4, 5});
  EXPECT_EQ(5, A.size());

  auto iter = A.begin();
  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(i + 1, *iter);
    ++iter;
  }
}

TEST(Array, CopyConstructor) {
  lib::array<int, 5> origin({5, 4, 3, 2, 1});
  lib::array<int, 5> cp(origin);

  EXPECT_EQ(origin.size(), cp.size());

  auto origin_iter = origin.begin();
  auto copy_iter = cp.begin();
  EXPECT_NE(origin_iter, copy_iter);
  for (; origin_iter != origin.end(); ++origin_iter, ++copy_iter) {
    EXPECT_EQ(*origin_iter, *copy_iter);
  }

  for (auto iter = cp.begin(); iter != cp.end(); ++iter) {
    *iter = 6;
  }

  for (auto iter = cp.begin(); iter != cp.end(); ++iter) {
    EXPECT_EQ(*iter, 6);
  }

  origin_iter = origin.begin();
  for (size_t i = 5; i > 0; --i, ++origin_iter) {
    EXPECT_EQ(i, *origin_iter);
  }
}

TEST(Array, MoveConstructor) {
  lib::array<double, 5> origin({1.1, 2.2, 3.3, 4.4, 5.5});
  lib::array<double, 5> moved(std::move(origin));

  EXPECT_EQ(5, moved.size());
  EXPECT_EQ(0, origin.size());

  auto i = 1;
  for (auto iter = moved.begin(); iter != moved.end(); ++iter, ++i) {
    EXPECT_DOUBLE_EQ(i * 1.1, *iter);
  }
}

TEST(Array, CopyAssignment) {
  lib::array<int, 3> A({1, 2, 3});
  lib::array<int, 3> origin(A);

  EXPECT_EQ(origin.size(), A.size());

  for (auto iter = A.begin(), oriter = origin.begin(); iter != A.end();
       ++iter, ++oriter) {
    EXPECT_EQ(*oriter, *iter);
  }
}

TEST(Array, MoveAssignment) {
  lib::array<int, 4> A({1, 2, 3});
  lib::array<int, 4> origin({4, 5, 6, 7});

  A = std::move(origin);

  EXPECT_EQ(4, A.size());
  EXPECT_EQ(4, origin.size());

  auto i = 4;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(Array, InitializerListAssignment) {
  lib::array<int, 5> A;

  A = {1, 2, 3, 4, 5};

  EXPECT_EQ(5, A.size());

  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(Array, ElementAccessAt) {
  lib::array<int, 4> A({7, 11, 13, 24});

  EXPECT_EQ(7, A.at(0));
  EXPECT_EQ(24, A.at(3));
  EXPECT_EQ(11, A.at(1));

  const lib::array<int, 4> const_A({7, 11, 13, 24});
  EXPECT_EQ(7, const_A.at(0));
  EXPECT_EQ(24, const_A.at(3));
  EXPECT_EQ(11, const_A.at(1));
}

TEST(Array, ElementAccessSquareBrackets) {
  lib::array<int, 4> myArray({7, 11, 13, 24});

  EXPECT_EQ(myArray[0], 7);
  EXPECT_EQ(myArray[1], 11);
  EXPECT_EQ(myArray[2], 13);
  EXPECT_EQ(myArray[3], 24);

  myArray[0] = 70;
  myArray[1] = 110;
  myArray[2] = 130;
  myArray[3] = 240;

  EXPECT_EQ(myArray[0], 70);
  EXPECT_EQ(myArray[1], 110);
  EXPECT_EQ(myArray[2], 130);
  EXPECT_EQ(myArray[3], 240);
}

TEST(Array, ElementAccessFront) {
  lib::array<char, 3> A({'a', 'b', 'c'});
  EXPECT_EQ('a', A.front());

  lib::array<double, 1> l2({5.5});
  EXPECT_DOUBLE_EQ(5.5, l2.front());

  lib::array<std::string, 4> l3({"School", "21", "by", "Sber"});
  EXPECT_EQ("School", l3.front());

  const lib::array<int, 4> const_A({7, 11, 13, 24});
  EXPECT_EQ(7, const_A.front());
}

TEST(Array, ElementAccessBack) {
  lib::array<char, 3> A({'a', 'b', 'c'});
  EXPECT_EQ('c', A.back());

  lib::array<double, 1> l2({5.5});
  EXPECT_DOUBLE_EQ(5.5, l2.back());

  lib::array<std::string, 4> l3({"School", "21", "by", "Sber"});
  EXPECT_EQ("Sber", l3.back());

  const lib::array<int, 4> const_A({7, 11, 13, 24});
  EXPECT_EQ(24, const_A.back());
}

TEST(Array, ElementAccessData) {
  const lib::array<int, 4> A({7, 11, 13, 24});
  const int* data_ptr = A.data();

  ASSERT_NE(data_ptr, nullptr);

  for (std::size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(A[i], data_ptr[i]);
  }

  lib::array<int, 4> B({7, 11, 13, 24});
  int* data_ptr_b = B.data();
  ASSERT_NE(data_ptr_b, nullptr);
  for (std::size_t i = 0; i < B.size(); ++i) {
    EXPECT_EQ(B[i], data_ptr_b[i]);
  }
}

TEST(Array, IteratorBeginEnd) {
  lib::array<int, 7> A({2, 4, 6, 8, 10, 12, 14});
  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(2 * i, *iter);
  }
  EXPECT_EQ(8, i);

  i = 1;
  for (auto iter = A.cbegin(); iter != A.cend(); ++iter, ++i) {
    EXPECT_EQ(2 * i, *iter);
  }
  EXPECT_EQ(8, i);
}

TEST(Array, CapacityEmpty) {
  lib::array<double, 0> A;
  ASSERT_TRUE(A.empty());
  lib::array<double, 7> B;
  ASSERT_FALSE(B.empty());
  lib::array<double, 3> C = {1.78, 0.99, -1.53};
  ASSERT_FALSE(C.empty());
}

TEST(Array, CapacitySize) {
  lib::array<int, 0> A;
  EXPECT_EQ(0, A.size());
  lib::array<double, 42> B;
  EXPECT_EQ(42, B.size());
}

TEST(Array, CapacityMaxSize) {
  lib::array<int, 4> const_A({7, 11, 13, 24});
  EXPECT_EQ(4, const_A.max_size());
}

TEST(Array, ModifiersSwap) {
  lib::array<int, 4> l1({1, 2, 3, 4});
  lib::array<int, 4> l2({4, 5, 6, 7});

  l1.swap(l2);

  EXPECT_EQ(4, l1.size());
  EXPECT_EQ(4, l2.size());

  auto iter = l2.begin();
  for (int i = 1; i <= 4; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }

  iter = l1.begin();
  for (int i = 4; i <= 7; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(Array, ModifiersSelfSwap) {
  lib::array<int, 4> A({1, 2, 3, 4});

  A.swap(A);

  EXPECT_EQ(4, A.size());

  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(Array, ModifiersFill) {
  lib::array<int, 4> A({7, 11, 13, 24});
  EXPECT_EQ(7, A.at(0));
  EXPECT_EQ(11, A.at(1));
  EXPECT_EQ(13, A.at(2));
  EXPECT_EQ(24, A.at(3));
  A.fill(7);
  EXPECT_EQ(7, A.at(0));
  EXPECT_EQ(7, A.at(1));
  EXPECT_EQ(7, A.at(2));
  EXPECT_EQ(7, A.at(3));
  lib::array<int, 4> B({7, 11, 13, 24});
  A = B;
  EXPECT_EQ(7, A[0]);
  EXPECT_EQ(11, A[1]);
  EXPECT_EQ(13, A[2]);
  EXPECT_EQ(24, A[3]);
  const lib::array<int, 4> B_const({7, 11, 13, 24});
  EXPECT_EQ(7, B_const[0]);
  EXPECT_EQ(11, B_const[1]);
  EXPECT_EQ(13, B_const[2]);
  EXPECT_EQ(24, B_const[3]);
}
