#include <gtest/gtest.h>

#include "../lib_containers.h"

TEST(List, DefaultConstructor) {
  lib::list<int> A;
  EXPECT_EQ(0, A.size());
}

TEST(List, ParameterizedConstructor) {
  lib::list<double> A(24);
  EXPECT_EQ(24, A.size());
  for (auto iter = A.begin(); iter != A.end(); ++iter) {
    EXPECT_DOUBLE_EQ(0, *iter);
  }
}

TEST(List, InitializerListConstructor) {
  lib::list<unsigned> A({1, 2, 3, 4, 5});
  EXPECT_EQ(5, A.size());

  auto iter = A.begin();
  for (size_t i = 0; i < A.size(); ++i) {
    EXPECT_EQ(i + 1, *iter);
    ++iter;
  }
}

TEST(List, CopyConstructor) {
  lib::list<int> origin({5, 4, 3, 2, 1});
  lib::list<int> cp(origin);

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

  origin_iter = std::move(origin.begin());
  for (size_t i = 5; i > 0; --i, ++origin_iter) {
    EXPECT_EQ(i, *origin_iter);
  }
}

TEST(List, MoveConstructor) {
  lib::list<double> origin({1.1, 2.2, 3.3, 4.4, 5.5});
  lib::list<double> moved(std::move(origin));

  EXPECT_EQ(5, moved.size());
  EXPECT_EQ(0, origin.size());

  auto i = 1;
  for (auto iter = moved.begin(); iter != moved.end(); ++iter, ++i) {
    EXPECT_DOUBLE_EQ(i * 1.1, *iter);
  }
}

TEST(List, CopyAssignment) {
  lib::list<int> A({1, 2, 3});
  lib::list<int> origin(A);

  EXPECT_EQ(origin.size(), A.size());

  for (auto iter = A.begin(), oriter = origin.begin(); iter != A.end();
       ++iter, ++oriter) {
    EXPECT_EQ(*oriter, *iter);
  }
}

TEST(List, MoveAssignment) {
  lib::list<int> A({1, 2, 3});
  lib::list<int> origin({4, 5, 6, 7});

  A = std::move(origin);

  EXPECT_EQ(4, A.size());
  EXPECT_EQ(0, origin.size());

  auto i = 4;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, InitializerListAssignment) {
  lib::list<int> A(25);

  A = {1, 2, 3, 4, 5};

  EXPECT_EQ(5, A.size());

  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ElementAccessFront) {
  lib::list<char> A({'a', 'b', 'c'});
  EXPECT_EQ('a', A.front());

  lib::list<double> l2({5.5});
  EXPECT_DOUBLE_EQ(5.5, l2.front());

  lib::list<std::string> l3({"School", "21", "by", "Sber"});
  EXPECT_EQ("School", l3.front());
}

TEST(List, ElementAccessBack) {
  lib::list<char> A({'a', 'b', 'c'});
  EXPECT_EQ('c', A.back());

  lib::list<double> l2({5.5});
  EXPECT_DOUBLE_EQ(5.5, l2.back());

  lib::list<std::string> l3({"School", "21", "by", "Sber"});
  EXPECT_EQ("Sber", l3.back());
}

TEST(List, Iterator) {
  lib::list<int> A({2, 4, 6, 8, 10, 12, 14});
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

TEST(List, CapacitySize) {
  lib::list<char> A;
  EXPECT_EQ(0, A.size());
  A = lib::list<char>(10);
  EXPECT_EQ(10, A.size());
}

TEST(List, CapacityEmpty) {
  lib::list<double> A;
  ASSERT_TRUE(A.empty());
  const lib::list<double> B(7);
  ASSERT_FALSE(B.empty());
  lib::list<double> C = {1.78, 0.99, -1.53};
  ASSERT_FALSE(C.empty());
  C = std::move(A);
  ASSERT_TRUE(C.empty());
}

TEST(List, ModifiersClear) {
  lib::list<char> A(10);
  A.clear();
  EXPECT_EQ(0, A.size());
}

TEST(List, ModifiersInsert) {
  lib::list<char> A({'b', 'b'});
  auto iter = A.insert(A.begin(), 'a');
  ++iter;
  ++iter;
  iter = A.insert(iter, 'o');
  A.insert(A.end(), 'a');

  EXPECT_EQ(5, A.size());
  std::string text = "aboba";
  auto i = 0;
  for (iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersErase) {
  lib::list<char> A({'x', 'a', 'r', 'b', 'o', 'b', 'r', 'a'});
  auto iter = A.begin();
  A.erase(iter);
  iter = A.begin();
  ++iter;
  A.erase(iter);
  iter = A.end();
  iter--;
  --iter;
  A.erase(iter);

  EXPECT_EQ(5, A.size());
  std::string text = "aboba";
  auto i = 0;
  for (iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersPushBack) {
  lib::list<int> A;

  for (int i = 0; i < 100; ++i) {
    A.push_back(i);
  }

  EXPECT_EQ(100, A.size());

  int i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersPopBack) {
  lib::list<char> A(10000);

  for (size_t i = 0; i < 7000; ++i) {
    A.pop_back();
  }

  EXPECT_EQ(3000, A.size());

  for (size_t i = 0; i < 3000; ++i) {
    A.pop_back();
  }
  EXPECT_EQ(0, A.size());
}

TEST(List, ModifiersPushFront) {
  lib::list<int> A;

  for (int i = 0; i < 100; ++i) {
    A.push_front(i);
  }

  EXPECT_EQ(100, A.size());

  int i = 99;
  for (auto iter = A.begin(); iter != A.end(); ++iter, --i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersSwap) {
  lib::list<int> l1({1, 2, 3});
  lib::list<int> l2({4, 5, 6, 7});

  l1.swap(l2);

  EXPECT_EQ(4, l1.size());
  EXPECT_EQ(3, l2.size());

  auto iter = l2.begin();
  for (int i = 1; i <= 3; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }

  iter = l1.begin();
  for (int i = 4; i <= 7; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersSelfSwap) {
  lib::list<int> A({1, 2, 3, 4});

  A.swap(A);

  EXPECT_EQ(4, A.size());

  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersMergeFullOrdered) {
  lib::list<int> l1({1, 2, 3, 4});
  lib::list<int> l2({5, 6, 7, 8});

  l1.merge(l2);

  EXPECT_EQ(8, l1.size());
  EXPECT_EQ(0, l2.size());

  auto iter = l1.begin();
  for (int i = 1; i < 9; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersMergeReversed) {
  lib::list<int> l1({5, 6, 7, 8});
  lib::list<int> l2({1, 2, 3, 4});

  l1.merge(l2);

  EXPECT_EQ(8, l1.size());
  EXPECT_EQ(0, l2.size());

  auto iter = l1.begin();
  for (int i = 1; i < 9; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersMergeShuffled) {
  lib::list<int> l1({2, 3, 4, 7});
  lib::list<int> l2({1, 5, 6, 8});

  l1.merge(l2);

  EXPECT_EQ(8, l1.size());
  EXPECT_EQ(0, l2.size());

  auto iter = l1.begin();
  for (int i = 1; i < 9; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersMergeEmptyRight) {
  lib::list<int> l1({1, 2, 3, 4, 5, 6, 7, 8});
  lib::list<int> l2;

  l1.merge(l2);

  EXPECT_EQ(8, l1.size());
  EXPECT_EQ(0, l2.size());

  auto iter = l1.begin();
  for (int i = 1; i < 9; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersMergeEmptyLeft) {
  lib::list<unsigned> l1;
  lib::list<unsigned> l2({1, 2, 3, 4, 5, 6, 7, 8});

  l1.merge(l2);

  EXPECT_EQ(8, l1.size());
  EXPECT_EQ(0, l2.size());

  auto iter = l1.begin();
  for (unsigned i = 1; i < 9; ++i, ++iter) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersSpliceFront) {
  lib::list<char> l1({'o', 'b', 'u', 's'});
  lib::list<char> l2({'a', 'b'});

  l1.splice(l1.begin(), l2);

  EXPECT_EQ(6, l1.size());
  EXPECT_EQ(0, l2.size());

  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = l1.begin(); iter != l1.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersSpliceBack) {
  lib::list<char> l1({'a', 'b', 'o'});
  lib::list<char> l2({'b', 'u', 's'});

  l1.splice(l1.end(), l2);

  EXPECT_EQ(6, l1.size());
  EXPECT_EQ(0, l2.size());

  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = l1.begin(); iter != l1.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersSpliceMiddle) {
  lib::list<char> l1({'a', 's'});
  lib::list<char> l2({'b', 'o', 'b', 'u'});

  auto iter = l1.begin();
  ++iter;

  l1.splice(iter, l2);

  EXPECT_EQ(6, l1.size());
  EXPECT_EQ(0, l2.size());

  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = l1.begin(); iter != l1.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersSpliceEmptyRight) {
  lib::list<char> l1({'a', 'b', 'o', 'b', 'u', 's'});
  lib::list<char> l2;

  auto iter = l1.begin();
  ++iter;

  l1.splice(iter, l2);

  EXPECT_EQ(6, l1.size());
  EXPECT_EQ(0, l2.size());

  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = l1.begin(); iter != l1.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersSpliceEmptyLeft) {
  lib::list<char> l1;
  lib::list<char> l2({'a', 'b', 'o', 'b', 'u', 's'});

  l1.splice(l1.begin(), l2);

  EXPECT_EQ(6, l1.size());
  EXPECT_EQ(0, l2.size());

  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = l1.begin(); iter != l1.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersReverseEmpty) {
  lib::list<char> A;

  A.reverse();

  EXPECT_EQ(0, A.size());
}

TEST(List, ModifiersReverse) {
  lib::list<char> A({'O', 'B', 'U', 'C', 'A', 'P', 'K'});

  A.reverse();

  EXPECT_EQ(7, A.size());

  std::string text = "KPACUBO";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersUniqueNOOP) {
  lib::list<char> A({'a', 'b', 'o', 'b', 'u', 's'});

  A.unique();

  EXPECT_EQ(6, A.size());
  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersUnique) {
  lib::list<char> A({'a', 'a', 'a', 'b', 'b', 'o', 'b', 'u', 's', 's', 's'});

  A.unique();

  EXPECT_EQ(6, A.size());
  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, ModifiersSortAlreadySorted) {
  lib::list<int> A({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

  A.sort();

  EXPECT_EQ(10, A.size());
  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersSortReversed) {
  lib::list<int> A({10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

  A.sort();

  EXPECT_EQ(10, A.size());
  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersSortShuffled) {
  lib::list<int> A({5, 6, 4, 10, 8, 2, 3, 7, 9, 1});

  A.sort();

  EXPECT_EQ(10, A.size());
  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, ModifiersSortEmpty) {
  lib::list<std::string> A;

  A.sort();

  EXPECT_EQ(0, A.size());
}

TEST(List, ModifiersSortZeroValues) {
  lib::list<double> A(10000);

  A.sort();

  EXPECT_EQ(10000, A.size());

  for (auto iter = A.begin(); iter != A.end(); ++iter) {
    EXPECT_DOUBLE_EQ(0, *iter);
  }
}

TEST(List, BonusModifiersInsertManyOnce) {
  lib::list<int> A({1, 2, 3});

  auto pos = A.begin();
  ++pos;
  pos = A.insert_many(pos, 4);

  EXPECT_EQ(4, *pos);

  EXPECT_EQ(4, A.size());

  int arr[] = {1, 4, 2, 3};
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(arr[i], *iter);
  }
}

TEST(List, BonusModifiersInsertManyMultiple) {
  lib::list<char> A({'a', 'b', 'c'});

  auto pos = A.begin();
  ++pos;
  ++pos;
  pos = A.insert_many(pos, 'd', 'e', 'f');

  EXPECT_EQ(*pos, 'd');

  EXPECT_EQ(6, A.size());

  char arr[] = {'a', 'b', 'd', 'e', 'f', 'c'};
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(arr[i], *iter);
  }

  pos = A.insert_many(A.begin(), 'a', 'b', std::move('o'), 'b', 'a');

  EXPECT_EQ(*pos, 'a');

  char extended_arr[] = {'a', 'b', 'o', 'b', 'a', 'a', 'b', 'd', 'e', 'f', 'c'};
  i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(extended_arr[i], *iter);
  }
}

TEST(List, BonusModifiersInsertManyNothing) {
  lib::list<double> A({1.1, 2.2, 3.3});
  auto pos = A.insert_many(A.begin());

  EXPECT_DOUBLE_EQ(*pos, 1.1);

  EXPECT_EQ(3, A.size());
  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_DOUBLE_EQ(i * 1.1, *iter);
  }
}

TEST(List, BonusModifiersInsertManyToEmpty) {
  lib::list<int> A;
  auto pos = A.insert_many(A.begin(), 1, 2, 3);

  EXPECT_EQ(*pos, 1);
  EXPECT_EQ(3, A.size());

  int i = 1;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(i, *iter);
  }
}

TEST(List, BonusModifiersInsertManyToEnd) {
  lib::list<char> A({'a', 'b', 'o'});
  auto pos = A.insert_many(A.end(), 'b', 'u', 's');
  EXPECT_EQ(*pos, 'b');

  EXPECT_EQ(6, A.size());
  std::string text = "abobus";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, BonusModifiersInsertManyBackOnce) {
  lib::list<int> A({0, 2, 4, 6});
  A.insert_many_back(8);

  EXPECT_EQ(5, A.size());
  auto iter = A.begin();
  for (size_t i = 0; i < A.size(); ++i, ++iter) {
    EXPECT_EQ(2 * i, *iter);
  }
}

TEST(List, BonusModifiersInsertManyBackMultiple) {
  lib::list<int> A({0, 1, 4, 9});
  A.insert_many_back(std::move(16), std::move(25), 36);

  EXPECT_EQ(7, A.size());
  auto iter = A.begin();
  for (size_t i = 0; i < A.size(); ++i, ++iter) {
    EXPECT_EQ(i * i, *iter);
  }
}

TEST(List, BonusModifiersInsertManyBackNothing) {
  lib::list<char> A({'c', 'o', 'o', 'l'});
  A.insert_many_back();

  EXPECT_EQ(4, A.size());
  std::string text = "cool";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, BonusModifiersInsertManyBackEmpty) {
  lib::list<char> A;
  A.insert_many_back('c', 'o', 'o', 'l');

  EXPECT_EQ(4, A.size());
  std::string text = "cool";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, BonusModifiersInsertManyFrontOnce) {
  lib::list<int> A({2, 4, 6, 8});
  A.insert_many_front(0);

  EXPECT_EQ(5, A.size());
  auto iter = A.begin();
  for (size_t i = 0; i < A.size(); ++i, ++iter) {
    EXPECT_EQ(2 * i, *iter);
  }
}

TEST(List, BonusModifiersInsertManyFrontMultiple) {
  lib::list<int> A({9, 16, 25, 36});
  A.insert_many_front(std::move(0), std::move(1), 4);

  EXPECT_EQ(7, A.size());
  auto iter = A.begin();
  for (size_t i = 0; i < A.size(); ++i, ++iter) {
    EXPECT_EQ(i * i, *iter);
  }
}

TEST(List, BonusModifiersInsertManyFrontNothing) {
  lib::list<char> A({'c', 'o', 'o', 'l'});
  A.insert_many_front();

  EXPECT_EQ(4, A.size());
  std::string text = "cool";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); ++iter, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}

TEST(List, BonusModifiersInsertManyFrontEmpty) {
  lib::list<char> A;
  A.insert_many_front('c', 'o', 'o', 'l');

  EXPECT_EQ(4, A.size());
  std::string text = "cool";
  size_t i = 0;
  for (auto iter = A.begin(); iter != A.end(); iter++, ++i) {
    EXPECT_EQ(text[i], *iter);
  }
}
