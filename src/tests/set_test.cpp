#include <gtest/gtest.h>

#include "../lib_containers.h"

TEST(Set, DefaultConstructor) {
  lib::set<int> emptySet;
  EXPECT_TRUE(emptySet.empty());
}

TEST(Set, InitializerListConstructor) {
  lib::set<int> initializedSet = {1, 2, 3};
  EXPECT_EQ(initializedSet.size(), 3);
  for (long unsigned int i = 1; i <= initializedSet.size(); i++) {
    EXPECT_TRUE(initializedSet.contains(i));
  }
  EXPECT_FALSE(initializedSet.contains(4));
}

TEST(Set, CopyConstructor) {
  lib::set<int> original = {1, 2, 3, 4, 5, 6};
  lib::set<int> copy(original);
  EXPECT_EQ(original.size(), 6);
  EXPECT_EQ(original.size(), copy.size());
  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));
  EXPECT_TRUE(copy.contains(3));
}

TEST(Set, MoveConstructor) {
  lib::set<int> OrSet = {1, 2, 3};
  lib::set<int> Copy = std::move(OrSet);
  EXPECT_EQ(Copy.size(), 3);
  EXPECT_EQ(OrSet.size(), 0);
  int i = 1;
  for (auto it = Copy.begin(); it != Copy.end(); it++, i++) {
    EXPECT_EQ(*it, i);
  }
  EXPECT_TRUE(OrSet.empty());
}

TEST(Set, MoveAssignmentOperator) {
  lib::set<int> origin = {1, 2, 3};
  lib::set<int> copy = {4, 5, 6, 7, 8, 9};
  copy = std::move(origin);
  EXPECT_EQ(origin.size(), 0);
  EXPECT_EQ(copy.size(), 3);
  auto j = 1;
  for (auto iter = copy.begin(); iter != copy.end(); ++iter, j++) {
    EXPECT_EQ(*iter, j);
  }
}

TEST(Set, IteratorsBeginEnd) {
  lib::set<int> mySet = {1, 2, 3, 4, 5};
  auto it = mySet.begin();
  int expectedValue = 1;
  while (it != mySet.end()) {
    EXPECT_EQ(*it, expectedValue);
    ++it;
    ++expectedValue;
  }
}

TEST(Set, CapacityEmpty) {
  lib::set<int> emptySet;
  lib::set<int> nonEmptySet = {1, 2, 3};
  EXPECT_TRUE(emptySet.empty());
  EXPECT_FALSE(nonEmptySet.empty());
}

TEST(Set, CapacitySize) {
  lib::set<int> emptyset;
  lib::set<double> setOfSize5 = {0, 1, 2, 3, 4};
  lib::set<int> setOfSize10 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  EXPECT_EQ(emptyset.size(), 0u);
  EXPECT_EQ(setOfSize5.size(), 5u);
  EXPECT_EQ(setOfSize10.size(), 10u);
}

TEST(Set, ModifiersClear) {
  lib::set<int> test_set = {1, 2, 3, 4, 5, 6};
  EXPECT_EQ(test_set.size(), 6);
  test_set.clear();
  EXPECT_EQ(test_set.size(), 0);
  EXPECT_TRUE(test_set.empty());
}

TEST(Set, ModifiersInsert) {
  std::pair<lib::set<int>::iterator, bool> result;
  lib::set<int> mySet;
  result = mySet.insert(42);
  EXPECT_TRUE(result.second);
  result = mySet.insert(42);
  EXPECT_FALSE(result.second);
  result = mySet.insert(10);
  EXPECT_TRUE(result.second);
  result = mySet.insert(20);
  EXPECT_EQ(*(result.first), 20);
  EXPECT_TRUE(result.second);
  auto it = mySet.begin();
  EXPECT_EQ(*it, 10);
}

TEST(Set, ModifiersErase) {
  lib::set<int> mySet;
  mySet.insert(1);
  mySet.insert(2);
  mySet.insert(3);
  mySet.insert(4);
  auto it = mySet.find(2);
  if (it != mySet.end()) {
    mySet.erase(it);
  }
  EXPECT_FALSE(mySet.find(2) != mySet.end());
  EXPECT_TRUE(mySet.find(1) != mySet.end());
  EXPECT_TRUE(mySet.find(3) != mySet.end());
  EXPECT_TRUE(mySet.find(4) != mySet.end());
}

TEST(Set, ModifiersSwap) {
  lib::set<int> set1 = {1, 2, 3};
  lib::set<int> set2 = {4, 5, 6};
  set1.swap(set2);
  EXPECT_TRUE(set1.find(1) == set1.end());
  EXPECT_TRUE(set1.find(2) == set1.end());
  EXPECT_TRUE(set1.find(3) == set1.end());
  EXPECT_TRUE(set2.find(4) == set2.end());
  EXPECT_TRUE(set2.find(5) == set2.end());
  EXPECT_TRUE(set2.find(6) == set2.end());
}

TEST(Set, ModifiersMerge) {
  lib::set<int> set1 = {1, 2, 3};
  lib::set<int> set2 = {3, 4, 5};
  EXPECT_TRUE(set1.find(1) != set1.end());
  EXPECT_TRUE(set1.find(2) != set1.end());
  EXPECT_TRUE(set1.find(3) != set1.end());
  set1.merge(set2);
  EXPECT_FALSE(set1.empty());
  EXPECT_TRUE(set1.find(1) != set1.end());
  EXPECT_TRUE(set1.find(2) != set1.end());
  EXPECT_TRUE(set1.find(3) != set1.end());
  EXPECT_TRUE(set1.find(4) != set1.end());
  EXPECT_TRUE(set1.find(5) != set1.end());
}

TEST(Set, LookupFind) {
  lib::set<int> mySet = {1, 2, 3, 4, 5};
  auto found1 = mySet.find(3);
  EXPECT_TRUE(found1 != mySet.end());
  auto found2 = mySet.find(6);
  EXPECT_TRUE(found2 == mySet.end());
}

TEST(Set, LookupContains) {
  lib::set<int> mySet = {1, 2, 3, 4, 5};
  EXPECT_TRUE(mySet.contains(3));
  EXPECT_FALSE(mySet.contains(6));
}

TEST(Set, BonusModifiersInsertMany) {
  lib::set<int> test({1, 2, 3, 3, 3, 4, 5});
  auto result = test.insert_many(3, 6, 5, 3, 4, 7, 8, 8, 9, 10);
  EXPECT_EQ(test.size(), 10);
  EXPECT_FALSE(result[0].second);
  EXPECT_TRUE(result[1].second);
  EXPECT_FALSE(result[2].second);
  EXPECT_FALSE(result[3].second);
  EXPECT_FALSE(result[4].second);
  EXPECT_TRUE(result[5].second);
  auto iter = test.end();
  for (int i = 10; i > 0; --i) {
    --iter;
    EXPECT_EQ(*iter, i);
  }
}

TEST(Set, BonusModifiersInsertMany2) {
  lib::set<int> lib_set = {1, 2, 3};
  lib::set<int> lib_exm = {0, 1, 2, 3, 4};

  lib_set.insert_many(4, 0);

  EXPECT_EQ(lib_set.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_set.begin(); lib_it != lib_set.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}

TEST(Set, BonusModifiersInsertMany3) {
  lib::set<char> lib_set = {'b', 'f', 'e'};
  lib::set<char> lib_exm = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};

  lib_set.insert_many('c', 'g', 'd', 'a');

  EXPECT_EQ(lib_set.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_set.begin(); lib_it != lib_set.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}

TEST(Set, BonusModifiersInsertMany4) {
  lib::set<std::string> lib_set = {"Cats", "Are"};
  lib::set<std::string> lib_exm = {"Are", "Best", "Cats", "The", "!"};

  lib_set.insert_many("The", "Best", "!");

  EXPECT_EQ(lib_set.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_set.begin(); lib_it != lib_set.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}

TEST(Set, BonusModifiersInsertMany5) {
  lib::set<double> lib_set = {2.13, 1.23};
  lib::set<double> lib_exm = {1.23, 2.13, 3.12, 4.12, 5.12};

  lib_set.insert_many(5.12, 3.12, 4.12);

  EXPECT_EQ(lib_set.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_set.begin(); lib_it != lib_set.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}
