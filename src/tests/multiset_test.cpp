#include <gtest/gtest.h>

#include "../lib_containersplus.h"

TEST(Multiset, DefaultConstructor) {
  lib::multiset<int> test;
  EXPECT_TRUE(test.empty());
}

TEST(Multiset, InitializerListConstructor) {
  lib::multiset<int> test_set = {1, 2, 2, 3, 3, 3};
  EXPECT_EQ(test_set.size(), 6);
  EXPECT_TRUE(test_set.contains(1));
  EXPECT_TRUE(test_set.contains(2));
  EXPECT_TRUE(test_set.contains(3));
  EXPECT_FALSE(test_set.contains(0));
  EXPECT_FALSE(test_set.contains(4));
}

TEST(Multiset, CopyConstructor) {
  lib::multiset<int> original = {1, 2, 2, 3, 3, 3};
  lib::multiset<int> copy(original);
  EXPECT_EQ(original.size(), 6);
  EXPECT_EQ(original.size(), copy.size());
  EXPECT_TRUE(copy.contains(1));
  EXPECT_TRUE(copy.contains(2));
  EXPECT_TRUE(copy.contains(3));
}

TEST(Multiset, MoveConstructor) {
  lib::multiset<int> original = {1, 2, 3, 1};
  lib::multiset<int> moved(std::move(original));

  EXPECT_TRUE(original.empty());
  EXPECT_EQ(moved.size(), 4);

  EXPECT_TRUE(moved.contains(1));
  EXPECT_TRUE(moved.contains(2));
  EXPECT_TRUE(moved.contains(3));
}

TEST(Multiset, MoveAssignmentOperator) {
  lib::multiset<int> source;
  source.insert(1);
  source.insert(2);
  source.insert(2);

  lib::multiset<int> destination;
  destination.insert(3);

  destination = std::move(source);

  EXPECT_EQ(source.size(), 0);
  EXPECT_FALSE(source.contains(1));
  EXPECT_FALSE(source.contains(2));

  EXPECT_EQ(destination.size(), 3);
  EXPECT_TRUE(destination.contains(1));
  EXPECT_TRUE(destination.contains(2));
  EXPECT_FALSE(destination.contains(3));
}

TEST(Multiset, IteratorsBeginEnd) {
  lib::multiset<int> test_set;
  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(3);
  int i = 1;
  for (auto it = test_set.begin(); it != test_set.end(); ++it, ++i) {
    EXPECT_EQ(*it, i);
  }
}

TEST(Multiset, CapacityEmptyTrue) {
  lib::multiset<int> test;
  EXPECT_TRUE(test.empty());
}

TEST(Multiset, CapacityEmptyFalse) {
  lib::multiset<int> test = {1};
  EXPECT_FALSE(test.empty());
}

TEST(Multiset, CapacitySize) {
  lib::multiset<int> test = {1};
  EXPECT_EQ(test.size(), 1);
}

TEST(Multiset, CapacitySizeZero) {
  lib::multiset<int> test;
  EXPECT_EQ(test.size(), 0);
}

TEST(Multiset, CapacitySize4) {
  lib::multiset<int> test_set;
  EXPECT_EQ(test_set.size(), 0);

  test_set.insert(1);
  test_set.insert(2);
  test_set.insert(2);
  test_set.insert(3);

  EXPECT_EQ(test_set.size(), 4);
}

TEST(Multiset, ModifiersClear) {
  lib::multiset<int> test_set = {1, 2, 2, 3, 3, 3};
  EXPECT_EQ(test_set.size(), 6);
  test_set.clear();
  EXPECT_EQ(test_set.size(), 0);
  EXPECT_TRUE(test_set.empty());
}

TEST(Multiset, ModifiersInsert) {
  lib::multiset<int> test;
  test.insert(1);
  EXPECT_FALSE(test.empty());
  EXPECT_EQ(test.size(), 1);
  test.insert(2);
  EXPECT_EQ(test.size(), 2);
  test.insert(2);
  EXPECT_EQ(test.size(), 3);
  test.insert(2);
  EXPECT_EQ(test.size(), 4);
}

TEST(Multiset, ModifiersErase) {
  lib::multiset<int> test_set = {1, 2, 2, 3, 3, 3};
  EXPECT_EQ(test_set.size(), 6);
  auto it = test_set.find(2);
  test_set.erase(it);
  EXPECT_EQ(test_set.size(), 5);
}

TEST(Multiset, ModifiersSwap) {
  lib::multiset<int> test_set1 = {1, 2, 2, 3, 3, 3};
  lib::multiset<int> test_set2 = {4, 5, 6};

  EXPECT_EQ(test_set1.size(), 6);
  EXPECT_EQ(test_set2.size(), 3);

  test_set1.swap(test_set2);
  EXPECT_EQ(test_set1.size(), 3);
  EXPECT_EQ(test_set2.size(), 6);
  EXPECT_TRUE(test_set1.contains(4));
  EXPECT_TRUE(test_set2.contains(1));
}

TEST(Multiset, ModifiersMerge) {
  lib::multiset<int> test_set1 = {1, 2, 2, 3, 3, 3};
  lib::multiset<int> test_set2 = {4, 5, 6};
  test_set1.merge(test_set2);
  EXPECT_EQ(test_set1.size(), 9);
  EXPECT_TRUE(test_set2.empty());
}

TEST(Multiset, LookupCount) {
  lib::multiset<int> multiset = {1, 2, 2, 3, 2, 4, 4, 4, 5};
  EXPECT_EQ(multiset.count(1), 1);
  EXPECT_EQ(multiset.count(2), 3);
  EXPECT_EQ(multiset.count(3), 1);
  EXPECT_EQ(multiset.count(4), 3);
  EXPECT_EQ(multiset.count(5), 1);
  EXPECT_EQ(multiset.count(6), 0);
}

TEST(Multiset, LookupFind) {
  lib::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  auto it1 = test_set.find(1);
  auto it2 = test_set.find(2);
  auto it3 = test_set.find(3);
  auto it4 = test_set.find(4);

  EXPECT_NE(it1, test_set.end());
  EXPECT_NE(it2, test_set.end());
  EXPECT_NE(it3, test_set.end());
  EXPECT_EQ(it4, test_set.end());
}

TEST(Multiset, LookupContains) {
  lib::multiset<int> test_set = {1, 2, 2, 3, 3, 3};

  EXPECT_TRUE(test_set.contains(1));
  EXPECT_TRUE(test_set.contains(2));
  EXPECT_TRUE(test_set.contains(3));
  EXPECT_FALSE(test_set.contains(0));
  EXPECT_FALSE(test_set.contains(4));
}

TEST(Multiset, LookupEqualRange) {
  lib::multiset<int> myMultiset = {1, 2, 2, 3, 4, 4, 5};
  auto range1 = myMultiset.equal_range(2);
  EXPECT_EQ(range1.first, myMultiset.lower_bound(2));
  EXPECT_EQ(range1.second, myMultiset.upper_bound(2));
  auto range2 = myMultiset.equal_range(4);
  EXPECT_EQ(range2.first, myMultiset.lower_bound(4));
  EXPECT_EQ(range2.second, myMultiset.upper_bound(4));
}

TEST(Multiset, LookupEqualRange4135) {
  const lib::multiset<int> one = {1, 1, 1, 1, 1, 1, 43, 413, 123, 4135};
  auto res = one.equal_range(4135);
  EXPECT_EQ(*(res.first), 4135);
  EXPECT_EQ(res.second, one.end());
}

TEST(Multiset, LookupLowerBound) {
  lib::multiset<int> my = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
  auto lb1 = my.lower_bound(2);
  EXPECT_NE(lb1, my.end());
  EXPECT_EQ(*lb1, 2);
  auto lb2 = my.lower_bound(5);
  EXPECT_EQ(lb2, my.end());
  auto lb3 = my.lower_bound(0);
  EXPECT_EQ(lb3, my.begin());
}

TEST(Multiset, LookupUpperBound) {
  lib::multiset<int> my = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};
  auto lb1 = my.upper_bound(1);
  EXPECT_NE(lb1, my.end());
  EXPECT_EQ(*lb1, 2);
  auto lb2 = my.upper_bound(5);
  EXPECT_EQ(lb2, my.end());
  auto lb3 = my.upper_bound(0);
  EXPECT_EQ(lb3, my.begin());
}

TEST(Multiset, BonusModifiersInsertMany) {
  lib::multiset<double> v = {2, -3, 20, -5, 1, -6};
  v.insert_many(8, 42, 26, 1, 1, 1);
  lib::multiset<double>::iterator x = v.lower_bound(8);
  --x;
  ASSERT_EQ(*x, 2);
}

TEST(Multiset, BonusModifiersInsertMany2) {
  lib::multiset<int> lib_multiset = {2, 2, 1, 3};
  lib::multiset<int> lib_exm = {0, 1, 2, 2, 3, 4, 4};

  lib_multiset.insert_many(4, 4, 0);

  EXPECT_EQ(lib_multiset.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_multiset.begin(); lib_it != lib_multiset.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}

TEST(Multiset, BonusModifiersInsertMany3) {
  lib::multiset<char> lib_multiset = {'b', 'b', 'f', 'f', 'e', 'e'};
  lib::multiset<char> lib_exm = {'a', 'b', 'b', 'c', 'd',
                                 'e', 'e', 'f', 'f', 'g'};

  lib_multiset.insert_many('c', 'g', 'd', 'a');

  EXPECT_EQ(lib_multiset.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_multiset.begin(); lib_it != lib_multiset.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}

TEST(Multiset, BonusModifiersInsertMany4) {
  lib::multiset<std::string> lib_multiset = {"Cats", "Are"};
  lib::multiset<std::string> lib_exm = {"Are", "Best", "Best", "Cats",
                                        "The", "The",  "!",    "!"};

  lib_multiset.insert_many("The", "The", "Best", "Best", "!", "!");

  EXPECT_EQ(lib_multiset.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_multiset.begin(); lib_it != lib_multiset.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}

TEST(Multiset, BonusModifiersInsertMany5) {
  lib::multiset<double> lib_multiset = {2.13, 2.13, 1.23};
  lib::multiset<double> lib_exm = {1.23, 2.13, 2.13, 3.12, 3.12,
                                   4.12, 4.12, 5.12, 5.12};

  lib_multiset.insert_many(5.12, 5.12, 3.12, 3.12, 4.12, 4.12);

  EXPECT_EQ(lib_multiset.size(), lib_exm.size());
  auto exm_it = lib_exm.begin();
  for (auto lib_it = lib_multiset.begin(); lib_it != lib_multiset.end();
       lib_it++, exm_it++) {
    EXPECT_EQ(*lib_it, *exm_it);
  }
}
