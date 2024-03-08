#include "gtest/gtest.h"
#include "../lru/lru.hpp"

using namespace sjtu;

TEST(DoubleListTest, InsertHeadTest) {
  double_list<int> list;
  list.insert_head(5);
  list.insert_head(10);
  ASSERT_EQ(*list.begin(), 10);
}

TEST(DoubleListTest, InsertTailTest) {
  double_list<int> list;
  list.insert_tail(5);
  list.insert_tail(10);
  auto it = list.begin();
  ++it;
  ASSERT_EQ(*it, 10);
}

TEST(DoubleListTest, DeleteHeadTest) {
  double_list<int> list;
  list.insert_head(5);
  list.insert_head(10);
  list.delete_head();
  ASSERT_EQ(*list.begin(), 5);
}

TEST(DoubleListTest, DeleteTailTest) {
  double_list<int> list;
  list.insert_tail(5);
  list.insert_tail(10);
  list.delete_tail();
  auto it = list.begin();
  ++it;
  ASSERT_EQ(it, list.end());
}

TEST(DoubleListTest, EmptyTest) {
  double_list<int> list;
  ASSERT_TRUE(list.empty());
  list.insert_head(5);
  ASSERT_FALSE(list.empty());
}

TEST(DoubleListTest, IteratorTest) {
  double_list<int> list;
  list.insert_tail(5);
  list.insert_tail(10);
  list.insert_tail(15);
  auto it = list.begin();
  ASSERT_EQ(*it, 5);
  ++it;
  ASSERT_EQ(*it, 10);
  --it;
  ASSERT_EQ(*it, 5);
}

TEST(DoubleListTest, EraseTest) {
  double_list<int> list;
  list.insert_tail(5);
  list.insert_tail(10);
  list.insert_tail(15);
  auto it = list.begin();
  ++it;
  it = list.erase(it);
  ASSERT_EQ(*it, 15);
  it = list.begin();
  ASSERT_EQ(*it, 5);
}