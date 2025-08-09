#include <gtest/gtest.h>

#include "../lib_containers.h"

TEST(Queue, DefaultConstructor) {
  lib::queue<int> A;

  EXPECT_EQ(A.empty(), true);
  EXPECT_EQ(A.size(), 0);
}

TEST(Queue, CopyConstructor) {
  lib::queue<int> A;
  A.push(1);
  lib::queue<int> B(A);
  B.pop();

  EXPECT_EQ(A.size(), 1);
  EXPECT_EQ(A.front(), 1);
  EXPECT_EQ(B.size(), 0);
}

TEST(Queue, MoveConstructor) {
  lib::queue<int> A;
  A.push(1);
  lib::queue<int> B(std::move(A));

  EXPECT_EQ(B.empty(), false);
  EXPECT_EQ(B.size(), 1);
  EXPECT_EQ(B.front(), 1);
  EXPECT_EQ(A.empty(), true);
}

TEST(Queue, MoveAssignment) {
  lib::queue<int> A;
  A.push(1);
  lib::queue<int> B = std::move(A);

  EXPECT_EQ(B.empty(), false);
  EXPECT_EQ(B.size(), 1);
  EXPECT_EQ(B.front(), 1);
  EXPECT_EQ(A.empty(), true);
}

TEST(Queue, InitializerListConstructor) {
  std::initializer_list<int> tmp{1, 2, 3};
  lib::queue<int> A(tmp);

  EXPECT_EQ(A.empty(), false);
  EXPECT_EQ(A.size(), 3);
  EXPECT_EQ(A.front(), 1);
  EXPECT_EQ(A.back(), 3);
}

TEST(Queue, ElementAccessFront) {
  std::initializer_list<int> tmp{1, 2, 3};
  lib::queue<int> A(tmp);

  EXPECT_EQ(A.front(), 1);
}

TEST(Queue, ElementAccessBack) {
  std::initializer_list<int> tmp{1, 2, 3};
  lib::queue<int> A(tmp);

  EXPECT_EQ(A.back(), 3);
}

TEST(Queue, CapacityEmptyTrue) {
  lib::queue<int> A;

  EXPECT_EQ(A.empty(), true);
}

TEST(Queue, CapacityEmptyFalse) {
  lib::queue<int> A;
  A.push(1);

  EXPECT_EQ(A.empty(), false);
}

TEST(Queue, CapacitySizeNull) {
  lib::queue<int> A;

  EXPECT_EQ(A.size(), 0);
}

TEST(Queue, CapacitySize) {
  std::initializer_list<int> tmp{1, 2, 3, 4, 5};
  lib::queue<int> A(tmp);

  EXPECT_EQ(A.size(), 5);
}

TEST(Queue, ModifiersPush) {
  lib::queue<int> A;
  A.push(2);

  EXPECT_EQ(A.front(), 2);
}

TEST(Queue, ModifiersPop) {
  lib::queue<int> A;
  A.push(2);
  A.pop();

  EXPECT_EQ(A.empty(), true);
}

TEST(Queue, ModifiersSwap) {
  lib::queue<int> A;
  lib::queue<int> B;
  A.push(2);
  A.swap(B);

  EXPECT_EQ(A.empty(), true);
  EXPECT_EQ(B.empty(), false);
  EXPECT_EQ(B.size(), 1);
  EXPECT_EQ(B.front(), 2);
}

TEST(Queue, BonusModifiersInsertManyBackOnce) {
  lib::queue<int> A({0, 1, 2});
  A.insert_many_back(3);

  EXPECT_EQ(4, A.size());

  for (int i = 0; i < 4; ++i, A.pop()) {
    EXPECT_EQ(i, A.front());
  }
}

TEST(Queue, BonusModifiersInsertManyBackMultiple) {
  lib::queue<int> A({0, 1, 4, 9});
  A.insert_many_back(std::move(16), std::move(25), 36);

  EXPECT_EQ(7, A.size());

  for (int i = 0; i < 7; ++i, A.pop()) {
    EXPECT_EQ(i * i, A.front());
  }
}

TEST(Queue, BonusModifiersInsertManyBackNothing) {
  lib::queue<int> A({1, 2, 3});
  A.insert_many_back();

  EXPECT_EQ(3, A.size());

  for (int i = 1; i < 4; ++i, A.pop()) {
    EXPECT_EQ(i, A.front());
  }
}

TEST(Queue, BonusModifiersInsertManyBackEmpty) {
  lib::queue<int> A;
  A.insert_many_back(1, 2, 3);

  EXPECT_EQ(3, A.size());

  for (int i = 1; i < 4; ++i, A.pop()) {
    EXPECT_EQ(i, A.front());
  }
}
