#include <gtest/gtest.h>

#include "../lib_containers.h"

TEST(Map, DefaultConstructor) {
  lib::map<int, int> test;
  EXPECT_EQ(test.empty(), true);
}

TEST(Map, InitializerListConstructor) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ(test.empty(), false);
  EXPECT_EQ(test.at(1), 2);
  EXPECT_EQ(test.at(2), 3);
  EXPECT_EQ(test.at(3), 4);
  EXPECT_EQ(test.at(4), 5);
}

TEST(Map, CopyConstructor) {
  lib::map<int, int> test1;
  test1.insert(1, 2);
  test1.insert(2, 3);
  test1.insert(3, 4);
  lib::map<int, int> test2(test1);
  EXPECT_EQ(test2.at(1), 2);
  EXPECT_EQ(test2.at(2), 3);
  EXPECT_EQ(test2.at(3), 4);
}

TEST(Map, MoveConstructor) {
  lib::map<int, int> test1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  lib::map<int, int> test2(std::move(test1));
  EXPECT_EQ(test1.size(), 0);
  EXPECT_EQ(test2.at(1), 2);
  EXPECT_EQ(test2.at(2), 3);
  EXPECT_EQ(test2.at(3), 4);
  EXPECT_EQ(test2.at(4), 5);
}

TEST(Map, MoveAssignmentOperator) {
  lib::map<int, int> test1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  lib::map<int, int> test2;
  test2 = std::move(test1);
  EXPECT_EQ(test1.size(), 0);
  EXPECT_EQ(test2.at(1), 2);
  EXPECT_EQ(test2.at(2), 3);
  EXPECT_EQ(test2.at(3), 4);
  EXPECT_EQ(test2.at(4), 5);
}

TEST(Map, ElementAccessAt) {
  lib::map<int, int> test;
  test.insert(1, 2);
  EXPECT_EQ(test.at(1), 2);
}

TEST(Map, ElementAccessAtMany) {
  lib::map<int, int> test;
  test.insert(1, 2);
  test.insert(2, 3);
  test.insert(3, 4);
  test.insert(-1, 5);
  EXPECT_EQ(test.at(3), 4);
}

TEST(Map, ElementAccessAtError) {
  lib::map<int, int> test;
  test.insert(1, 2);
  EXPECT_THROW(test.at(2), std::out_of_range);
}

TEST(Map, ElementAccessSquareBrackets) {
  lib::map<int, int> test;
  test.insert(1, 2);
  EXPECT_EQ(test[1], 2);
}

TEST(Map, ElementAccessSquareBracketsMany) {
  lib::map<int, int> test;
  test.insert(1, 2);
  test.insert(2, 3);
  test.insert(3, 4);
  test.insert(-1, 5);
  EXPECT_EQ(test[3], 4);
  test[4] = 5;
  test[5] = 6;
  EXPECT_EQ(test[5], 6);
}

TEST(Map, IteratorBegin) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ((*test.begin()).first, 1);
  EXPECT_EQ((*test.begin()).second, 2);
}

TEST(Map, IteratorEnd) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ((*(--test.end())).first, 4);
  EXPECT_EQ((*(--test.end())).second, 5);
}

TEST(Map, Iterations) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  auto i = test.begin();
  EXPECT_EQ((*i).first, 1);
  EXPECT_EQ((*i).second, 2);
  i++;
  EXPECT_EQ((*i).first, 2);
  EXPECT_EQ((*i).second, 3);
  i++;
  EXPECT_EQ((*i).first, 3);
  EXPECT_EQ((*i).second, 4);
  i++;
  EXPECT_EQ((*i).first, 4);
  EXPECT_EQ((*i).second, 5);
}

TEST(Map, CapacityEmpty) {
  lib::map<int, int> test;
  EXPECT_EQ(test.empty(), true);
}

TEST(Map, CapacityEmptyFalse) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ(test.empty(), false);
}

TEST(Map, CapacitySizeZero) {
  lib::map<int, int> test;
  EXPECT_EQ(test.size(), 0);
}

TEST(Map, CapacitySize) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ(test.size(), 4);
}

TEST(Map, ModifiersClear) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  test.clear();
  EXPECT_EQ(test.size(), 0);
}

TEST(Map, ModifiersInsertValueType) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  auto pair = std::make_pair(9, 9);
  test.insert(pair);
  EXPECT_EQ(test[9], 9);
}

TEST(Map, ModifiersInsertKeyAndObj) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  test.insert(9, 9);
  EXPECT_EQ(test[9], 9);
}

TEST(Map, ModifiersInsertOrAssign) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  test.insert_or_assign(9, 9);
  EXPECT_EQ(test[9], 9);
  test.insert_or_assign(9, 10);
  EXPECT_EQ(test[9], 10);
}

TEST(Map, ModifiersErase) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  test.erase(test.begin());
  EXPECT_EQ(test.contains(1), false);
}

TEST(Map, ModifiersSwap) {
  lib::map<int, int> test1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  lib::map<int, int> test2({{1, 10}, {2, 3}, {3, 4}, {4, 5}});
  test1.swap(test2);
  EXPECT_EQ(test1[1], 10);
  EXPECT_EQ(test2[1], 2);
}

TEST(Map, ModifiersMerge) {
  lib::map<int, int> test1({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  lib::map<int, int> test2({{5, 6}, {6, 7}});
  test1.merge(test2);
  EXPECT_EQ(test1.size(), 6);
  EXPECT_EQ(test1[1], 2);
  EXPECT_EQ(test1[5], 6);
  EXPECT_EQ(test1[6], 7);
}

TEST(Map, LookupContains) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ(test.contains(1), true);
}

TEST(Map, LookupContainsFalse) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  EXPECT_EQ(test.contains(5), false);
}

TEST(Map, BonusModifiersInsertMany) {
  lib::map<int, int> test({{1, 2}, {2, 3}, {3, 4}, {4, 5}});
  test.insert_many(std::make_pair(5, 6), std::make_pair(7, 8),
                   std::make_pair(9, 10));
  EXPECT_EQ(test.size(), 7);
  EXPECT_EQ(test[5], 6);
  EXPECT_EQ(test[7], 8);
  EXPECT_EQ(test[9], 10);
  EXPECT_EQ(test[1], 2);
}

TEST(Map, BonusModifiersInsertManySchool21) {
  lib::map<int, std::string> lib_map = {{2, "21"}, {4, "Sber"}};
  lib::map<int, std::string> lib_exm = {
      {1, "School"}, {2, "21"}, {3, "by"}, {4, "Sber"}};
  lib_map.insert_many(std::make_pair(1, "School"), std::make_pair(3, "by"));
  EXPECT_EQ(lib_map.size(), lib_exm.size());
  auto it_map = lib_map.begin();
  auto it_exm = lib_exm.begin();
  while (it_map != lib_map.end() && it_exm != lib_exm.end()) {
    EXPECT_EQ((*it_map).first, (*it_exm).first);
    EXPECT_EQ((*it_map).second, (*it_exm).second);
    ++it_map;
    ++it_exm;
  }
}
