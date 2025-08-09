#include <gtest/gtest.h>

#include "../lib_containers.h"

TEST(Stack, DefaultConstructor) {
  lib::stack<int> A;

  EXPECT_EQ(A.empty(), true);
  EXPECT_EQ(A.size(), 0);
}

TEST(Stack, CopyConstructor) {
  lib::stack<int> A;
  A.push(1);
  lib::stack<int> B(A);
  B.pop();

  EXPECT_EQ(A.size(), 1);
  EXPECT_EQ(A.top(), 1);
  EXPECT_EQ(B.size(), 0);
}

TEST(Stack, MoveConstructor) {
  lib::stack<int> A;
  A.push(1);
  lib::stack<int> B(std::move(A));

  EXPECT_EQ(B.empty(), false);
  EXPECT_EQ(B.size(), 1);
  EXPECT_EQ(B.top(), 1);
  EXPECT_EQ(A.empty(), true);
}

TEST(Stack, MoveAssignment) {
  lib::stack<int> A;
  A.push(1);
  lib::stack<int> B = std::move(A);

  EXPECT_EQ(B.empty(), false);
  EXPECT_EQ(B.size(), 1);
  EXPECT_EQ(B.top(), 1);
  EXPECT_EQ(A.empty(), true);
}

TEST(Stack, InitializerListConstructor) {
  std::initializer_list<int> tmp{1, 2, 3};
  lib::stack<int> A(tmp);

  EXPECT_EQ(A.empty(), false);
  EXPECT_EQ(A.size(), 3);
  EXPECT_EQ(A.top(), 3);
  A.pop();
  EXPECT_EQ(A.top(), 2);
  A.pop();
  EXPECT_EQ(A.top(), 1);
  A.pop();
  EXPECT_EQ(A.empty(), true);
}

TEST(Stack, ElementAccessTop) {
  std::initializer_list<int> tmp{1, 2, 3};
  lib::stack<int> A(tmp);

  EXPECT_EQ(A.top(), 3);
}

TEST(Stack, CapacityEmptyTrue) {
  lib::stack<int> A;

  EXPECT_EQ(A.empty(), true);
}

TEST(Stack, CapacityEmptyFalse) {
  lib::stack<int> A;
  A.push(1);

  EXPECT_EQ(A.empty(), false);
}

TEST(Stack, CapacitySizeNull) {
  lib::stack<int> A;

  EXPECT_EQ(A.size(), 0);
}

TEST(Stack, CapacitySize) {
  std::initializer_list<int> tmp{1, 2, 3, 4, 5};
  lib::stack<int> A(tmp);

  EXPECT_EQ(A.size(), 5);
}

TEST(Stack, ModifiersPush) {
  lib::stack<int> A;
  A.push(2);

  EXPECT_EQ(A.top(), 2);
}

TEST(Stack, ModifiersPop) {
  lib::stack<int> A;
  A.push(2);
  A.push(3);
  A.pop();

  EXPECT_EQ(A.empty(), false);
  EXPECT_EQ(A.top(), 2);
}

TEST(Stack, ModifiersSwap) {
  lib::stack<int> A;
  lib::stack<int> B;
  A.push(2);
  A.swap(B);

  EXPECT_EQ(A.empty(), true);
  EXPECT_EQ(B.empty(), false);
  EXPECT_EQ(B.size(), 1);
  EXPECT_EQ(B.top(), 2);
}

TEST(Stack, BonusModifiersInsertManyFrontOnce) {
  lib::stack<int> A({0, 2, 4, 6});
  A.insert_many_front(8);

  EXPECT_EQ(5, A.size());
  for (int i = 4; i >= 0; --i, A.pop()) {
    EXPECT_EQ(2 * i, A.top());
  }
}

TEST(Stack, BonusModifiersInsertManyFrontMultiple) {
  lib::stack<int> A({0, 1, 4, 9});
  A.insert_many_front(std::move(16), std::move(25), 36);

  EXPECT_EQ(7, A.size());
  for (int i = 6; i >= 0; --i, A.pop()) {
    EXPECT_EQ(i * i, A.top());
  }
}

TEST(Stack, BonusModifiersInsertManyFrontNothing) {
  lib::stack<int> A({1, 2, 3});
  A.insert_many_front();

  EXPECT_EQ(3, A.size());
  for (int i = 3; i > 0; --i, A.pop()) {
    EXPECT_EQ(i, A.top());
  }
}

TEST(Stack, BonusModifiersInsertManyFrontEmpty) {
  lib::stack<int> A;
  A.insert_many_front(1, 2, 3);

  EXPECT_EQ(3, A.size());
  for (int i = 3; i > 0; --i, A.pop()) {
    EXPECT_EQ(i, A.top());
  }
}
